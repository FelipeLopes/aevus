#include "opencl.hpp"
#include "cl_context.hpp"
#include "CL/cl.h"
#include <system_error>

namespace render {

OpenCL::OpenCL() {
    cl_uint numPlatforms;
    cl_int ret = clGetPlatformIDs(0, NULL, &numPlatforms);
    if (ret != CL_SUCCESS) {
        auto ec = std::error_code(ret, std::generic_category());
        throw std::system_error(ec, "Could not get number of OpenCL platforms.");
    }
}

CLContext OpenCL::getContext() {
    return CLContext();
}

}