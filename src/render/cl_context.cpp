#include "cl_context.hpp"
#include "cl_buffer.hpp"
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

CLBuffer CLContext::createReadOnlyBuffer(size_t size) {
    return CLBuffer(context, CL_MEM_READ_ONLY, size);
}

CLBuffer CLContext::createWriteOnlyBuffer(size_t size) {
    return CLBuffer(context, CL_MEM_WRITE_ONLY, size);
}

CLContext::~CLContext() {
    clReleaseContext(context);
}

}