#pragma once

#include <CL/cl.h>
#include <stdexcept>
#include <system_error>
#include <vector>

namespace render {

class CLBuffer {
public:
    CLBuffer(cl_context clContext, cl_command_queue clCommandQueue,
        cl_mem_flags clMemFlags, size_t size);
    ~CLBuffer();
    template <typename T>
    void write(const std::vector<T> data) {
        if (data.size()*sizeof(T) > size) {
            throw std::runtime_error("OpenCL buffer too small for data");
        }
        cl_int ret = clEnqueueWriteBuffer(commandQueue, memObject, CL_TRUE,
            0, data.size()*sizeof(T), data.data(), 0, NULL, NULL);
        if (ret != CL_SUCCESS) {
            auto ec = std::error_code(ret, std::generic_category());
            throw std::system_error(ec, "Could not write to OpenCL buffer");
        }
    }
private:
    cl_command_queue commandQueue;
    cl_mem memObject;
    size_t size;
};

}