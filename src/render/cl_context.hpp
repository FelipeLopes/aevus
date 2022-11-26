#pragma once

#include <CL/cl.h>
#include "cl_buffer.hpp"
#include "cl_executable.hpp"
#include "cl_queue.hpp"

namespace render {

class CLContext {
public:
    CLContext(cl_device_id clDeviceId);
    CLQueue createCommandQueue();
    CLExecutable createExecutable(std::string name, std::string filename);
    ~CLContext();
    cl_context context;
private:
    cl_device_id deviceId;
};

}