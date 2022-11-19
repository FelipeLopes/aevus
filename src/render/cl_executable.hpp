#pragma once

#include <CL/cl.h>
#include <string>

namespace render {

class CLExecutable {
public:
    CLExecutable(cl_context clContext, std::string filename);
private:
    cl_program program;
    cl_kernel kernel;
};

}