#pragma once

#include <CL/cl.h>
#include "cl_buffer.hpp"
#include "cl_queue.hpp"

namespace render {

class CLContext {
public:
    CLContext(cl_device_id clDeviceId);
    CLQueue createCommandQueue();
    CLBuffer createReadOnlyBuffer(size_t size);
    CLBuffer createWriteOnlyBuffer(size_t size);
    ~CLContext();
private:
    cl_context context;
    cl_device_id deviceId;
};

}