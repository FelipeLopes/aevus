#include "cl_executable.hpp"
#include "cl_queue.hpp"
#include <CL/cl.h>
#include <fstream>
#include <sstream>

namespace render {

CLExecutable::CLExecutable(std::string name, cl_context clContext,
    cl_device_id clDeviceId, std::string filename)
{
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
    ret = clBuildProgram(program, 1, &clDeviceId, NULL, NULL, NULL);
    if (ret != CL_SUCCESS) {
        auto ec = std::error_code(ret, std::generic_category());
        throw std::system_error(ec, "Could not build OpenCL program");
    }
    kernel = clCreateKernel(program, name.c_str(), &ret);
    if (ret != CL_SUCCESS) {
        auto ec = std::error_code(ret, std::generic_category());
        throw std::system_error(ec, "Could not create OpenCL kernel");
    }
}

void CLExecutable::run(const CLQueue& clQueue, const size_t globalWorkSize,
    const size_t localWorkSize) {

    cl_int ret = clEnqueueNDRangeKernel(clQueue.commandQueue, kernel, 1, NULL,
        &globalWorkSize, &localWorkSize, 0, NULL, NULL);
    if (ret != CL_SUCCESS) {
        auto ec = std::error_code(ret, std::generic_category());
        throw std::system_error(ec, "Could not run OpenCL kernel");
    }
}

CLExecutable::~CLExecutable() {
    clReleaseKernel(kernel);
    clReleaseProgram(program);
}

}