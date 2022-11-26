#pragma once

#include "cl_buffer.hpp"
#include "cl_queue.hpp"
#include <CL/cl.h>
#include <string>

namespace render {

class CLExecutable {
public:
    CLExecutable(std::string name, cl_context clContext,
        cl_device_id clDeviceId, std::string filename);
    ~CLExecutable();
    template <typename T>
    void setArg(unsigned int argIndex, const T& arg);
    void run(const CLQueue& clQueue, const size_t globalWorkSize, const size_t localWorkSize);
private:
    cl_program program;
    cl_kernel kernel;
};

template <typename T>
void CLExecutable::setArg(unsigned int argIndex, const T& arg) {
    cl_int ret = clSetKernelArg(kernel, argIndex, sizeof(T), &arg);
    if (ret != CL_SUCCESS) {
        auto ec = std::error_code(ret, std::generic_category());
        throw std::system_error(ec, "Could not set OpenCL kernel argument");
    }
}

template <>
inline void CLExecutable::setArg(unsigned int argIndex, const CLBuffer& clBuffer) {
    cl_int ret = clSetKernelArg(kernel, argIndex, sizeof(cl_mem), clBuffer.memoryObject());
    if (ret != CL_SUCCESS) {
        auto ec = std::error_code(ret, std::generic_category());
        throw std::system_error(ec, "Could not set OpenCL kernel argument");
    }
}

}