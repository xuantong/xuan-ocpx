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
#include "tracer_req.pb.h"
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
const std::array<std::underlying_type_t<tracer::ActionType>, 3> VALID_ACTIONTYPE_VALUES = {
    static_cast<std::underlying_type_t<tracer::ActionType>>(tracer::ActionType::CLICK), //点击
    static_cast<std::underlying_type_t<tracer::ActionType>>(tracer::ActionType::EXPOSURE), //展现
    static_cast<std::underlying_type_t<tracer::ActionType>>(tracer::ActionType::CALLBACK) //回调
};

// 定义移动设备OS类型枚举
enum class OSType {
  Other = 0,
  iOS = 1,
  Android = 2
};

enum class BizType {
  ELEME_CPS = 0,
  ELEME_FIRST_CALL = 1,
  MEITUAN_OCPA = 2
};

inline tracer::TracerReq extract2TracerReqProto(HttpRequest &req) {
  tracer::TracerReq data;

  // 从HttpRequest中提取参数，并设置到Protobuf消息
  std::string cqid = req.GetParam("cqid");
  checkCondition(!cqid.empty(), "Missing required parameter: cqid");
  data.set_cqid(cqid);

  std::string action_str = req.GetParam("action");
  tracer::ActionType action = EnumUtil::StringToEnum(
      action_str,
      tracer::ActionType::UNKNOWN,
      VALID_ACTIONTYPE_VALUES
  );
  checkCondition(action != tracer::ActionType::UNKNOWN,
                 "Missing required parameter: action, real value is " + action_str);
  data.set_action(action);

  std::string action_id = req.GetParam("action_id");
  checkCondition(!action_id.empty(), "Missing required parameter: action_id");
  data.set_action_id(action_id);

  // 设置其他可选参数
  if (!req.GetParam("request_id").empty()) {
    data.set_request_id(req.GetParam("request_id"));
  }
  if (!req.GetParam("rta_id").empty()) {
    data.set_rta_id(req.GetParam("rta_id"));
  }
  if (!req.GetParam("advertiser_id").empty()) {
    data.set_advertiser_id(req.GetParam("advertiser_id"));
  }
  if (!req.GetParam("campaign_id").empty()) {
    data.set_campaign_id(req.GetParam("campaign_id"));
  }
  if (!req.GetParam("ad_id").empty()) {
    data.set_ad_id(req.GetParam("ad_id"));
  }
  if (!req.GetParam("cid").empty()) {
    data.set_cid(req.GetParam("cid"));
  }
  if (!req.GetParam("os").empty()) {
    data.set_os(req.GetParam("os"));
  }
  if (!req.GetParam("imei").empty()) {
    data.set_imei(req.GetParam("imei"));
  }
  if (!req.GetParam("imei_md5").empty()) {
    data.set_imei_md5(req.GetParam("imei_md5"));
  }
  if (!req.GetParam("oaid").empty()) {
    data.set_oaid(req.GetParam("oaid"));
  }
  if (!req.GetParam("oaid_md5").empty()) {
    data.set_oaid_md5(req.GetParam("oaid_md5"));
  }
  if (!req.GetParam("android_id").empty()) {
    data.set_android_id(req.GetParam("android_id"));
  }
  if (!req.GetParam("android_id_md5").empty()) {
    data.set_android_id_md5(req.GetParam("android_id_md5"));
  }
  if (!req.GetParam("idfa").empty()) {
    data.set_idfa(req.GetParam("idfa"));
  }
  if (!req.GetParam("idfa_md5").empty()) {
    data.set_idfa_md5(req.GetParam("idfa_md5"));
  }
  if (!req.GetParam("caid").empty()) {
    data.set_caid(req.GetParam("caid"));
  }
  if (!req.GetParam("callback").empty()) {
    data.set_callback(req.GetParam("callback"));
  }
  if (!req.GetParam("mts").empty()) {
    data.set_mts(std::stoull(req.GetParam("mts")));
  }
  if (!req.GetParam("ip").empty()) {
    data.set_ip(req.GetParam("ip"));
  }
  if (!req.GetParam("ua").empty()) {
    data.set_ua(req.GetParam("ua"));
  }
  if (!req.GetParam("url").empty()) {
    data.set_url(req.GetParam("url"));
  }

  return data;
}

class TracerHttpHandler {
 private:
  static logpp::Logger logger_;
  LevelDBWrapper &level_db_wrapper_;
  RedisClient &redisClient_;

 public:
  TracerHttpHandler(LevelDBWrapper &level_db_wrapper, RedisClient &redisClient) : level_db_wrapper_(level_db_wrapper),
                                                                                  redisClient_(redisClient) {}

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
