#pragma once

#include "cl_context.hpp"
#include <CL/cl.h>
#include <stdexcept>
#include <system_error>
#include <vector>

namespace render {

template <typename T>
class CLBuffer {
public:
    CLBuffer(const CLContext& clContext, const CLQueue& clQueue,
        cl_mem_flags clMemFlags, size_t size);
    virtual ~CLBuffer();
    void write(const std::vector<T>& data);
    void read(std::vector<T>& data);
    const cl_mem* memoryObject() const;
private:
    cl_command_queue commandQueue;
    cl_mem memObject;
    size_t size;
};

template <typename T>
CLBuffer<T>::CLBuffer(const CLContext& clContext, const CLQueue& clQueue,
    cl_mem_flags clMemFlags, size_t size_): commandQueue(clQueue.commandQueue), size(size_) {

    cl_int ret;
    memObject = clCreateBuffer(clContext.context, clMemFlags, size*sizeof(T), NULL, &ret);
    if (ret != CL_SUCCESS) {
        auto ec = std::error_code(ret, std::generic_category());
        throw std::system_error(ec, "Could not create OpenCL buffer");
    }
}

template <typename T>
CLBuffer<T>::~CLBuffer() {
    clReleaseMemObject(memObject);
}

template <typename T>
void CLBuffer<T>::write(const std::vector<T>& data) {
    if (data.size() > size) {
        throw std::runtime_error("OpenCL buffer too small for data");
    }
    cl_int ret = clEnqueueWriteBuffer(commandQueue, memObject, CL_TRUE,
        0, data.size()*sizeof(T), data.data(), 0, NULL, NULL);
    if (ret != CL_SUCCESS) {
        auto ec = std::error_code(ret, std::generic_category());
        throw std::system_error(ec, "Could not write to OpenCL buffer");
    }
}

template <typename T>
void CLBuffer<T>::read(std::vector<T>& data) {
    data.resize(size);
    cl_int ret = clEnqueueReadBuffer(commandQueue, memObject, CL_TRUE, 0, size*sizeof(T),
        data.data(), 0, NULL, NULL);
    if (ret != CL_SUCCESS) {
        auto ec = std::error_code(ret, std::generic_category());
        throw std::system_error(ec, "Could not read from OpenCL buffer");
    }
}

template <typename T>
const cl_mem* CLBuffer<T>::memoryObject() const {
    return &memObject;
}

template <typename T>
class ReadOnlyCLBuffer: public CLBuffer<T> {
public:
    ReadOnlyCLBuffer(const CLContext& clContext, const CLQueue& clQueue, size_t size);
};

template <typename T>
ReadOnlyCLBuffer<T>::ReadOnlyCLBuffer(const CLContext& clContext,
    const CLQueue& clQueue, size_t size):
    CLBuffer<T>(clContext, clQueue, CL_MEM_READ_ONLY, size) { }

template <typename T>
class WriteOnlyCLBuffer: public CLBuffer<T> {
public:
    WriteOnlyCLBuffer(const CLContext& clContext, const CLQueue& clQueue, size_t size);
};

template <typename T>
WriteOnlyCLBuffer<T>::WriteOnlyCLBuffer(const CLContext& clContext,
    const CLQueue& clQueue, size_t size):
    CLBuffer<T>(clContext, clQueue, CL_MEM_WRITE_ONLY, size) { }

template <typename T>
class ReadWriteCLBuffer: public CLBuffer<T> {
public:
    ReadWriteCLBuffer(const CLContext& clContext, const CLQueue& clQueue, size_t size);
};

template <typename T>
ReadWriteCLBuffer<T>::ReadWriteCLBuffer(const CLContext& clContext,
    const CLQueue& clQueue, size_t size):
    CLBuffer<T>(clContext, clQueue, CL_MEM_READ_WRITE, size) { }

}