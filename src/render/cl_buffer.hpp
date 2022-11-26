#pragma once

#include <CL/cl.h>
#include <stdexcept>
#include <system_error>
#include <vector>

namespace render {

template <typename T>
class CLBuffer {
public:
    CLBuffer(cl_context clContext, cl_command_queue clCommandQueue,
        cl_mem_flags clMemFlags, size_t size);
    ~CLBuffer();
    void write(const std::vector<T>& data);
    void read(std::vector<T>& data);
    const cl_mem* memoryObject() const;
private:
    cl_command_queue commandQueue;
    cl_mem memObject;
    size_t size;
};

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
CLBuffer<T>::CLBuffer(cl_context clContext, cl_command_queue clCommandQueue,
    cl_mem_flags clMemFlags, size_t size_): commandQueue(clCommandQueue), size(size_) {

    cl_int ret;
    memObject = clCreateBuffer(clContext, clMemFlags, size*sizeof(T), NULL, &ret);
    if (ret != CL_SUCCESS) {
        auto ec = std::error_code(ret, std::generic_category());
        throw std::system_error(ec, "Could not create OpenCL buffer");
    }
}

template <typename T>
const cl_mem* CLBuffer<T>::memoryObject() const {
    return &memObject;
}

template <typename T>
CLBuffer<T>::~CLBuffer() {
    clReleaseMemObject(memObject);
}

}