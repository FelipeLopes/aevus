#pragma once

#include <CL/cl.h>
#include "cl_buffer.hpp"
#include "cl_executable.hpp"
#include "cl_queue.hpp"

namespace render {

class CLContext {
public:
    CLContext(cl_device_id clDeviceId);
    CLQueue createCommandQueue();
    template <typename T>
    CLBuffer<T> createReadOnlyBuffer(const CLQueue& queue, size_t size) {
        return CLBuffer<T>(context, queue.commandQueue, CL_MEM_READ_ONLY, size*sizeof(T));
    }
    template <typename T>
    CLBuffer<T> createWriteOnlyBuffer(const CLQueue& queue, size_t size) {
        return CLBuffer<T>(context, queue.commandQueue, CL_MEM_WRITE_ONLY, size*sizeof(T));
    }
    template <typename T>
    CLBuffer<T> createReadWriteBuffer(const CLQueue& queue, size_t size) {
        return CLBuffer<T>(context, queue.commandQueue, CL_MEM_READ_WRITE, size*sizeof(T));
    }
    CLExecutable createExecutable(std::string name, std::string filename);
    ~CLContext();
private:
    cl_context context;
    cl_device_id deviceId;
};

}