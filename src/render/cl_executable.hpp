#pragma once

#include "cl_buffer.hpp"
#include "cl_context.hpp"
#include "cl_queue.hpp"
#include <CL/cl.h>
#include <string>

namespace render {

class CLExecutable {
public:
    CLExecutable(const CLQueuedContext& clContext, std::string name, std::string filename);
    ~CLExecutable();
    template <typename T>
    void setArg(unsigned int argIndex, const T& arg);
    template <typename T>
    void setBufferArg(unsigned int argIndex, const CLBuffer<T>& arg);
    void runBlocking(const size_t globalWorkSize, const size_t localWorkSize);
    const CLQueuedContext& context;
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

template <typename T>
void CLExecutable::setBufferArg(unsigned int argIndex, const CLBuffer<T>& clBuffer) {
    cl_int ret = clSetKernelArg(kernel, argIndex, sizeof(cl_mem), clBuffer.memoryObject());
    if (ret != CL_SUCCESS) {
        auto ec = std::error_code(ret, std::generic_category());
        throw std::system_error(ec, "Could not set OpenCL kernel argument");
    }
}

}