#pragma once

#include "cl_buffer.hpp"
#include "cl_context.hpp"
#include "cl_event.hpp"
#include "cl_queue.hpp"
#include <CL/cl.h>
#include <string>

namespace clwrap {

class CLKernelBufferArg;

class CLExecutable {
public:
    CLExecutable(CLQueuedContext& clContext, std::string name,
        const char* sourceStart, const char* sourceEnd);
    ~CLExecutable();
    template <typename T>
    void setArg(unsigned int argIndex, const T& arg);
    template <typename T>
    void setBufferArg(unsigned int argIndex, const CLBuffer<T>* arg);
    void runBlocking(const size_t globalWorkSize, const size_t localWorkSize);
    std::shared_ptr<CLEvent> runAsync(const size_t globalWorkSize, const size_t localWorkSize);
    CLQueuedContext& context;
    struct LazyArg {
        CLKernelBufferArg* bufferArg;
        std::vector<uint8_t> data;
        LazyArg(CLKernelBufferArg* bufferArg_, std::vector<uint8_t>& data_):
            bufferArg(bufferArg_), data(data_) { }
    };
    std::vector<LazyArg> lazyArgs;
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
void CLExecutable::setBufferArg(unsigned int argIndex, const CLBuffer<T>* clBuffer) {
    auto memoryObject = clBuffer ? clBuffer->memoryObject() : NULL;
    cl_int ret = clSetKernelArg(kernel, argIndex, sizeof(cl_mem), memoryObject);
    if (ret != CL_SUCCESS) {
        auto ec = std::error_code(ret, std::generic_category());
        throw std::system_error(ec, "Could not set OpenCL kernel argument");
    }
}

}