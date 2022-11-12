#include "cl_context.hpp"
#include <CL/cl.h>
#include <system_error>

CLContext::CLContext(cl_device_id clDeviceId) {
    cl_int ret;
    context = clCreateContext(NULL, 1, &clDeviceId, NULL, NULL, &ret);
    if (ret != CL_SUCCESS) {
        auto ec = std::error_code(ret, std::generic_category());
        throw std::system_error(ec, "Could not create OpenCL context");
    }
}

CLContext::~CLContext() {
    clReleaseContext(context);
}