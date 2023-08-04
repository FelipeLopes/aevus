#pragma once

#include <CL/cl.h>

namespace clwrap {

class CLQueue {
public:
    CLQueue(cl_context clContext, cl_device_id clDeviceId);
    ~CLQueue();
    cl_command_queue commandQueue;
};

}