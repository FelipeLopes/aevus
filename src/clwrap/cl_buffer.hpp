#pragma once

#include "cl_context.hpp"
#include <CL/cl.h>
#include <functional>
#include <stdexcept>
#include <system_error>
#include <vector>

namespace clwrap {

template <typename T>
class CLBuffer {
public:
    CLBuffer(const CLContext& clContext, const CLQueue& clQueue,
        cl_mem_flags clMemFlags, size_t size);
    CLBuffer(const CLContext& clContext, const CLQueue& clQueue,
        cl_mem_flags clMemFlags, std::function<void(std::vector<T>&)> f);
    virtual ~CLBuffer();
    void write(const std::vector<T>& data);
    std::shared_ptr<CLEvent> writeAsync(const std::vector<T>& data);
    void read(std::vector<T>& data);
    size_t length() const;
    const cl_mem* memoryObject() const;
private:
    cl_command_queue commandQueue;
    cl_mem memObject;
    size_t size;
};

template <typename T>
CLBuffer<T>::CLBuffer(const CLContext& clContext, const CLQueue& clQueue,
    cl_mem_flags clMemFlags, size_t size_): commandQueue(clQueue.commandQueue), size(size_)
{
    cl_int ret;
    memObject = clCreateBuffer(clContext.context, clMemFlags, size*sizeof(T), NULL, &ret);
    if (ret != CL_SUCCESS) {
        auto ec = std::error_code(ret, std::generic_category());
        throw std::system_error(ec, "Could not create OpenCL buffer");
    }
}

template <typename T>
CLBuffer<T>::CLBuffer(const CLContext& clContext, const CLQueue& clQueue,
    cl_mem_flags clMemFlags, std::function<void(std::vector<T>&)> f):
    commandQueue(clQueue.commandQueue)
{
    std::vector<T> arr;
    f(arr);
    size = arr.size();
    cl_int ret;
    memObject = clCreateBuffer(clContext.context, clMemFlags, size*sizeof(T), NULL, &ret);
    if (ret != CL_SUCCESS) {
        auto ec = std::error_code(ret, std::generic_category());
        throw std::system_error(ec, "Could not create OpenCL buffer");
    }
    write(arr);
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
std::shared_ptr<CLEvent> CLBuffer<T>::writeAsync(const std::vector<T>& data) {
    cl_event event;
    cl_int ret = clEnqueueWriteBuffer(commandQueue, memObject, CL_FALSE,
        0, data.size()*sizeof(T), data.data(), 0, NULL, &event);
    if (ret != CL_SUCCESS) {
        auto ec = std::error_code(ret, std::generic_category());
        throw std::system_error(ec, "Could not write to OpenCL buffer");
    }
    return std::make_shared<CLEvent>(event);
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
size_t CLBuffer<T>::length() const {
    return size;
}

template <typename T>
const cl_mem* CLBuffer<T>::memoryObject() const {
    return &memObject;
}

}