#pragma once

#include <CL/cl.h>
#include "cl_queue.hpp"

namespace clwrap {

class CLContext {
public:
    CLContext(cl_device_id clDeviceId);
    CLQueue createCommandQueue();
    virtual ~CLContext();
    cl_context context;
    cl_device_id deviceId;
};

class CLQueuedContext: public CLContext {
public:
    CLQueuedContext(cl_device_id clDeviceId);
    CLQueue defaultQueue;
};

}