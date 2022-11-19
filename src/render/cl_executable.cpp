#include "cl_executable.hpp"
#include <CL/cl.h>
#include <fstream>
#include <sstream>

namespace render {

CLExecutable::CLExecutable(cl_context clContext, std::string filename) {
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer<<file.rdbuf();
    auto source = buffer.str();
    auto sArr = source.c_str();
    auto len = source.size();
    cl_int ret;
    program = clCreateProgramWithSource(clContext, 1, &sArr, &len, &ret);
    if (ret != CL_SUCCESS) {
        auto ec = std::error_code(ret, std::generic_category());
        throw std::system_error(ec, "Could not create OpenCL program");
    }
}

}