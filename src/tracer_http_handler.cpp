//
// Created by Dell on 2024/3/25 0025.
//
#include "tracer_http_handler.h"


logpp::Logger TracerHttpHandler::logger_ = logpp::Logger("TracerHttpHandler");

int TracerHttpHandler::tracer(HttpRequest *req, HttpResponse *resp) {
  logger_.info("TracerHttpHandler tracer");
  // 转换出对应对象数据
  tracer::req::TracerReq tracerReq = extract2TracerReqProto(*req);
  // 序列化 TracerReq 对象为 JSON 字符串
  std::string serialized;
  tracerReq.SerializeToString(&serialized);
  // 将元数据hash掉
  std::string hash_key = string_utils::hash(serialized);
  // 将这个对象转换并存储
  level_db_wrapper_.Put(hash_key, serialized);
  // 将数据缓存到redis上，保持一天，一般来说单次请求，当日内一般会转化返回，如果三天未转化则放弃
  redisClient_.set(hash_key, serialized, RedisClient::ONE_DAY * 3);
  // 根据配置文件提取回传数据对象
  std::unique_ptr<tracer::config::AdLinkConfig> tracerConfig =
      level_db_wrapper_.ProtoBufGet<tracer::config::AdLinkConfig>("config");

  std::string async_req_url ;
  if(tracerReq.action()==tracer::req::ActionType::EXPOSURE_ACTION){
    // 曝光数据渲染
    std::string render_data =  tracerConfig->ext_info().exposuretemplatelink();
    // 根据请求参数替换模板变量
    async_req_url = MacroVariableUtil::replace(render_data, req->query_params);
  }else if(tracerReq.action()==tracer::req::ActionType::CLICK_ACTION) {
    // 点击数据渲染
    std::string render_data = tracerConfig->ext_info().clicktemplatelink();
    // 根据请求参数替换模板变量
    async_req_url = MacroVariableUtil::replace(render_data, req->query_params);
  }




  return 200;
}

int TracerHttpHandler::callback(HttpRequest *req, HttpResponse *resp) {
    logger_.info("TracerHttpHandler callback");
    tracer::callback::TracerCallBack tracerCallbackReq = extract2TracerCallBackReqProto(*req);
    std::unique_ptr<tracer::req::TracerReq> tracerReq = level_db_wrapper_.ProtoBufGet<tracer::req::TracerReq>(tracerCallbackReq.id());
    if (tracerReq == nullptr) {
        logger_.error("TracerHttpHandler callback tracerReq is null");
        return 500;
    }

    return 200;
}