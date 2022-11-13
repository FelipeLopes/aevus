#pragma once

#include <CL/cl.h>

namespace render {

class CLBuffer {
public:
    CLBuffer(cl_context clContext, cl_command_queue clCommandQueue,
        cl_mem_flags clMemFlags, size_t size);
    ~CLBuffer();
private:
    cl_command_queue commandQueue;
    cl_mem_flags memFlags;
    cl_mem memObject;
};

}