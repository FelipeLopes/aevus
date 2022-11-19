#include "cl_executable.hpp"
#include <CL/cl.h>
#include <fstream>
#include <sstream>

namespace render {

CLExecutable::CLExecutable(std::string name, cl_context clContext,
    cl_device_id clDeviceId, std::string filename) {

    std::ifstream file(filename);
    std::stringstream buffer;
    buffer<<file.rdbuf();
    auto source = buffer.str();
    auto sArr = source.c_str();
    auto len = source.size();
    cl_int ret;
    program = clCreateProgramWithSource(clContext, 1, &sArr, &len, &ret);
    if (ret != CL_SUCCESS) {
        auto ec = std::error_code(ret, std::generic_category());
        throw std::system_error(ec, "Could not create OpenCL program");
    }
    ret = clBuildProgram(program, 1, &clDeviceId, NULL, NULL, NULL);
    if (ret != CL_SUCCESS) {
        auto ec = std::error_code(ret, std::generic_category());
        throw std::system_error(ec, "Could not build OpenCL program");
    }
    kernel = clCreateKernel(program, name.c_str(), &ret);
    if (ret != CL_SUCCESS) {
        auto ec = std::error_code(ret, std::generic_category());
        throw std::system_error(ec, "Could not create OpenCL kernel");
    }
}

void CLExecutable::setArg(unsigned int argIndex, const CLBuffer& clBuffer) {
    cl_int ret;
    ret = clSetKernelArg(kernel, argIndex, sizeof(cl_mem), clBuffer.memoryObject());
    if (ret != CL_SUCCESS) {
        auto ec = std::error_code(ret, std::generic_category());
        throw std::system_error(ec, "Could not set OpenCL kernel argument");
    }
}

CLExecutable::~CLExecutable() {
    clReleaseKernel(kernel);
    clReleaseProgram(program);
}

}