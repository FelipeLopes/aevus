#pragma once

#include "cl_buffer.hpp"
#include <CL/cl.h>
#include <string>

namespace render {

class CLExecutable {
public:
    CLExecutable(std::string name, cl_context clContext,
        cl_device_id clDeviceId, std::string filename);
    ~CLExecutable();
    void setArg(unsigned int argIndex, const CLBuffer& clBuffer);
private:
    cl_program program;
    cl_kernel kernel;
};

}