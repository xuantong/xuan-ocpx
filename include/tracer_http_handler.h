//
// Created by Dell on 2024/3/25 0025.
//

#ifndef XUANOCPX_TRACER_HTTP_HANDLER_H
#define XUANOCPX_TRACER_HTTP_HANDLER_H
#pragma once

#include "hv/HttpService.h"
#include "hv/HttpMessage.h"
#include "hv/json.hpp"
#include <string>
#include <logpp/logpp.hpp>
#include "hv/hstring.h"
#include "string_utils.h"
#include "enum_util.hpp"
#include "common_utils.hpp"
#include "leveldb_wrapper.hpp"
#include "redis_client.hpp"
/**
 * 参考自之前tracer的链接
 * 主接口：/thirdparty/general?action=xxx&cqid=xxx&h1=__ACTION_ID__&h2=__REQUEST_ID__&h3=__RTA_ID__&h4=__ADVERTISER_ID__&h5=__CAMPAIGN_ID__&h6=__AD_ID__&h7=__CID__&h8=__OS__&h9=__IMEI__&h10=__IMEI_MD5__&h11=__OAID__&h12=__OAID_MD5__&h13=__ANDROID_ID__&h14=__ANDROID_ID_MD5__&h15=__IDFA__&h16=__IDFA_MD5__&h17=__CAID__&h18=__CALLBACK__&h19=__ACTION_TYPE__&h20=__MTS__&h21=__MEDIUM_SOURCE__&h22=__IP__&h23=__UA__&valid_event_type=xxx
 * action:					用户行为类型。1：点击，2：曝光
 * cqid:					对应的链接ID
 * h1:__ACTION_ID__			曝光/点击等行为的ID，唯一对应用户每次操作
 * h2:__REQUEST_ID__		请求ID（对应问询参竞请求的ID）
 * h3:__RTA_ID__
 * h4:__ADVERTISER_ID__		广告主ID
 * h5:__CAMPAIGN_ID__		广告计划ID
 * h6:__AD_ID__				广告ID
 * h7:__CID__				广告创意ID
 * h8:__OS__				移动设备OS类型。0：其它，1：IOS，2：安卓
 * h9:__IMEI__				安卓IMEI原值
 * h10:__IMEI_MD5__			安卓IMEI MD5值
 * h11:__OAID__				安卓OAID原值
 * h12:__OAID_MD5__			安卓OAID MD5值
 * h13:__ANDROID_ID__		安卓AndroidId原值
 * h14:__ANDROID_ID_MD5__	安卓AndroidId MD5值
 * h15:__IDFA__				IOS IDFA原值
 * h16:__IDFA_MD5__			IOS IDFA MD值
 * h17:__CAID__				在IOS14后IDFA被禁用后的IOS设备标识（广协标准），原值，格式：version_caid，可能有多个，逗号分隔
 * h18:__CALLBACK__			回调信息，编码一次的URL
 * h19:__ACTION_TYPE__		用户行为类型。1：点击，2：曝光
 * h20:__MTS__				用户行为时间戳，自1970年起的毫秒数
 * h21:__MEDIUM_SOURCE__	请求媒体，固定值：27
 * h22:__IP__				用户IP地址
 * h23:__UA__				客户端UA，需要Encode一次
 * valid_event_type:		认可的转化类型：1激活2唤端UV 3成单
 */

// 使用nlohmann::json命名空间
using json = nlohmann::json;

// 定义用户行为类型枚举
enum class ActionType {
    Click = 1,   //点击
    Exposure = 2, //展现
    CallBack = 3, //回调事件
    Unknown = -1  //未知回调类型
};

const std::array<std::underlying_type_t<ActionType>, 3> VALID_ACTIONTYPE_VALUES = {
        static_cast<std::underlying_type_t<ActionType>>(ActionType::Click),
        static_cast<std::underlying_type_t<ActionType>>(ActionType::Exposure),
        static_cast<std::underlying_type_t<ActionType>>(ActionType::CallBack)
};

// 定义移动设备OS类型枚举
enum class OSType {
    Other = 0,
    iOS = 1,
    Android = 2
};

/**
 * tracer 请求对象，标准数据请求
 */
struct TracerReq {
    // 用户行为类型
    ActionType action;
    // 对应的链接ID
    std::string cqid;
    // 曝光/点击等行为的ID，唯一对应用户每次操作
    std::string action_id;
    // 请求ID（对应问询参竞请求的ID）
    std::string request_id;
    // RTA ID
    std::string rta_id;
    // 广告主ID
    std::string advertiser_id;
    // 广告计划ID
    std::string campaign_id;
    // 广告ID
    std::string ad_id;
    // 广告创意ID
    std::string cid;
    // 移动设备OS类型，不同平台的os处理不完全一样，此处仅做字符抽取
    std::string os;
    // 安卓IMEI原值
    std::string imei;
    // 安卓IMEI MD5值
    std::string imei_md5;
    // 安卓OAID原值
    std::string oaid;
    // 安卓OAID MD5值
    std::string oaid_md5;
    // 安卓AndroidId原值
    std::string android_id;
    // 安卓AndroidId MD5值
    std::string android_id_md5;
    // IOS IDFA原值
    std::string idfa;
    // IOS IDFA MD5值
    std::string idfa_md5;
    // 在IOS14后IDFA被禁用后的IOS设备标识（广协标准），原值
    std::string caid;
    // 回调信息，编码一次的URL
    std::string callback;
    // 用户行为时间戳，自1970年起的毫秒数
    uint64_t mts;
    // 用户IP地址
    std::string ip;
    // 客户端UA，需要Encode一次
    std::string ua;
    // req_get_uri ,当此请求完整的url地址
    std::string url;
};

// 为TracerReq结构体定义到json的转换
static void to_json(json &j, const TracerReq &req) {
    j = json{
            {"action",         req.action},
            {"cqid",           req.cqid},
            {"action_id",      req.action_id},
            {"request_id",     req.request_id},
            {"rta_id",         req.rta_id},
            {"advertiser_id",  req.advertiser_id},
            {"campaign_id",    req.campaign_id},
            {"ad_id",          req.ad_id},
            {"cid",            req.cid},
            {"os",             req.os},
            {"imei",           req.imei},
            {"imei_md5",       req.imei_md5},
            {"oaid",           req.oaid},
            {"oaid_md5",       req.oaid_md5},
            {"android_id",     req.android_id},
            {"android_id_md5", req.android_id_md5},
            {"idfa",           req.idfa},
            {"idfa_md5",       req.idfa_md5},
            {"caid",           req.caid},
            {"callback",       req.callback},
            {"mts",            req.mts},
            {"ip",             req.ip},
            {"ua",             req.ua},
            {"url",            req.url}
    };
}

static void from_json(const json &j, TracerReq &req) {
    j.at("action").get_to(req.action);
    j.at("cqid").get_to(req.cqid);
    j.at("action_id").get_to(req.action_id);
    j.at("request_id").get_to(req.request_id);
    j.at("rta_id").get_to(req.rta_id);
    j.at("advertiser_id").get_to(req.advertiser_id);
    j.at("campaign_id").get_to(req.campaign_id);
    j.at("ad_id").get_to(req.ad_id);
    j.at("cid").get_to(req.cid);
    j.at("os").get_to(req.os);
    j.at("imei").get_to(req.imei);
    j.at("imei_md5").get_to(req.imei_md5);
    j.at("oaid").get_to(req.oaid);
    j.at("oaid_md5").get_to(req.oaid_md5);
    j.at("android_id").get_to(req.android_id);
    j.at("android_id_md5").get_to(req.android_id_md5);
    j.at("idfa").get_to(req.idfa);
    j.at("idfa_md5").get_to(req.idfa_md5);
    j.at("caid").get_to(req.caid);
    j.at("callback").get_to(req.callback);
    j.at("mts").get_to(req.mts);
    j.at("ip").get_to(req.ip);
    j.at("ua").get_to(req.ua);
    j.at("uri").get_to(req.url);
}

inline TracerReq extractTracerReq(HttpRequest &req) {
    TracerReq data;
    // cqid和action_id是必传参数，如果没有提供，抛出异常
    data.cqid = req.GetParam("cqid");
    checkCondition(!data.cqid.empty(), "Missing required parameter: cqid");

    data.action = EnumUtil::StringToEnum(
            req.GetParam("action"),
            ActionType::Unknown,
            VALID_ACTIONTYPE_VALUES
    );

    checkCondition(data.action != ActionType::Unknown,
                   "Missing required parameter: action ,real value is " + req.GetParam("action"));

    data.action_id = req.GetParam("action_id");
    data.request_id = req.GetParam("request_id");
    data.rta_id = req.GetParam("rta_id");
    data.advertiser_id = req.GetParam("advertiser_id");
    data.campaign_id = req.GetParam("campaign_id");
    data.ad_id = req.GetParam("ad_id");
    data.cid = req.GetParam("cid");
    data.os = req.GetParam("os");
    data.imei = req.GetParam("imei");
    data.imei_md5 = req.GetParam("imei_md5");
    data.oaid = req.GetParam("oaid");
    data.oaid_md5 = req.GetParam("oaid_md5");
    data.android_id = req.GetParam("android_id");
    data.android_id_md5 = req.GetParam("android_id_md5");
    data.idfa = req.GetParam("idfa");
    data.idfa_md5 = req.GetParam("idfa_md5");
    data.caid = req.GetParam("caid");
    data.callback = req.GetParam("callback");
    data.mts = std::stoull(req.GetParam("mts", "0"));
    data.ip = req.GetParam("ip");
    data.ua = req.GetParam("ua");
    data.url = req.url;

    return data;
}

class TracerHttpHandler {
private:
    static logpp::Logger logger_;
    LevelDBWrapper &level_db_wrapper_;


public:
    TracerHttpHandler(LevelDBWrapper &level_db_wrapper) : level_db_wrapper_(level_db_wrapper) {}

    ~TracerHttpHandler() = default;

    /**
     * 数据初步跟踪逻辑
     * @param req
     * @param resp
     * @return
     */
    int tracer(HttpRequest *req, HttpResponse *resp);

    /**
     * 数据回调逻辑
     * @param req
     * @param resp
     * @return
     */
    int callback(HttpRequest *req, HttpResponse *resp);
};

#endif //XUANOCPX_TRACER_HTTP_HANDLER_H
