#pragma once

#include <CL/cl.h>
namespace render {

class CLQueue {
public:
    CLQueue(cl_context clContext, cl_device_id clDeviceId);
    ~CLQueue();
private:
    cl_command_queue commandQueue;
};

}