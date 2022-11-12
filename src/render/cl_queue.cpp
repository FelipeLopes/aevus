#include "cl_queue.hpp"
#include <CL/cl.h>
#include <system_error>

namespace render {

CLQueue::CLQueue(cl_context clContext, cl_device_id clDeviceId) {
    cl_int ret;
    commandQueue = clCreateCommandQueueWithProperties(clContext, clDeviceId, NULL, &ret);
    if (ret != CL_SUCCESS) {
        auto ec = std::error_code(ret, std::generic_category());
        throw std::system_error(ec, "Could not create OpenCL command queue");
    }
}

CLQueue::~CLQueue() {
    clReleaseCommandQueue(commandQueue);
}

}