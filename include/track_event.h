//
// Created by aaa99 on 2024/4/9.
//

#ifndef XUANOCPX_TRACK_EVENT_H
#define XUANOCPX_TRACK_EVENT_H

#include <vector>
#include <string>
#include "tracer_req.pb.h"
#include "tracer_callback.pb.h"

/**
 *  跟踪事件处理逻辑
 */
class TrackEvent {
public:
    virtual bool event(tracer::req ::TracerReq trackCallback) = 0;
    virtual std::vector<tracer::callback::TracerCallBack> createCallbackTask(const tracer::req ::TracerReq& adLog) = 0;
};

#endif //XUANOCPX_TRACK_EVENT_H
