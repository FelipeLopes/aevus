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
    CLBuffer createReadOnlyBuffer(const CLQueue& queue, size_t size);
    CLBuffer createWriteOnlyBuffer(const CLQueue& queue, size_t size);
    CLExecutable createExecutable(std::string name, std::string filename);
    ~CLContext();
private:
    cl_context context;
    cl_device_id deviceId;
};

}