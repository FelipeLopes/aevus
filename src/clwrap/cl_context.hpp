#pragma once

#include <CL/cl.h>
#include "cl_event.hpp"
#include "cl_queue.hpp"
#include <boost/asio/thread_pool.hpp>
#include <functional>
#include <memory>

namespace clwrap {

class CLContext {
public:
    CLContext(cl_device_id clDeviceId);
    CLQueue createCommandQueue();
    void setEventCallback(std::shared_ptr<CLEvent> event, CLEvent::Status status,
        std::function<void()> f);
    static void clCallback(cl_event clEvent, cl_int clStatus, void* object) {
        CLContext* context = reinterpret_cast<CLContext*>(object);
        auto status = CLEvent::convertCLStatus(clStatus);
        context->eventCallbacksLock.lock();
        auto it = std::find_if(context->eventCallbacks.begin(),
            context->eventCallbacks.end(), [clEvent, status](auto cb){
                return cb.event->clEvent == clEvent && cb.status == status;
            });
        if (it != context->eventCallbacks.end()) {
            auto cb = *it;
            context->eventCallbacks.erase(it);
            context->eventCallbacksLock.unlock();
            cb.f();
        } else {
            context->eventCallbacksLock.unlock();
        }
    }
    virtual ~CLContext();
    cl_context context;
    cl_device_id deviceId;
    boost::asio::thread_pool callbackPool;
    std::mutex eventCallbacksLock;
    std::vector<CLEventCallback> eventCallbacks;
};

class CLQueuedContext: public CLContext {
public:
    CLQueuedContext(cl_device_id clDeviceId);
    CLQueue defaultQueue;
};

}