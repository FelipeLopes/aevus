#include "cl_buffer.hpp"
#include <CL/cl.h>
#include <system_error>

namespace render {

CLBuffer::CLBuffer(cl_context clContext, cl_mem_flags clMemFlags, size_t size):
    memFlags(clMemFlags) {

    cl_int ret;
    memObject = clCreateBuffer(clContext, memFlags, size, NULL, &ret);
    if (ret != CL_SUCCESS) {
        auto ec = std::error_code(ret, std::generic_category());
        throw std::system_error(ec, "Could not create OpenCL buffer");
    }
}

CLBuffer::~CLBuffer() {
    clReleaseMemObject(memObject);
}

}