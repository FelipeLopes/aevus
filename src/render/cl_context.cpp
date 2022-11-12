#include "cl_context.hpp"
#include "cl_queue.hpp"
#include <CL/cl.h>
#include <system_error>

namespace render {

CLContext::CLContext(cl_device_id clDeviceId): deviceId(clDeviceId) {
    cl_int ret;
    context = clCreateContext(NULL, 1, &deviceId, NULL, NULL, &ret);
    if (ret != CL_SUCCESS) {
        auto ec = std::error_code(ret, std::generic_category());
        throw std::system_error(ec, "Could not create OpenCL context");
    }
}

CLQueue CLContext::createCommandQueue() {
    return CLQueue(context, deviceId);
}

CLContext::~CLContext() {
    clReleaseContext(context);
}

}