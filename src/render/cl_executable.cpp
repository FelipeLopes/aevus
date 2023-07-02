#include "cl_executable.hpp"
#include "cl_context.hpp"
#include "cl_queue.hpp"
#include <CL/cl.h>
#include <fstream>
#include <sstream>

namespace render {

CLExecutable::CLExecutable(const CLQueuedContext& clContext, std::string name, std::string filename):
    context(clContext)
{
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer<<file.rdbuf();
    auto source = buffer.str();
    auto sArr = source.c_str();
    auto len = source.size();
    cl_int ret;
    program = clCreateProgramWithSource(clContext.context, 1, &sArr, &len, &ret);
    if (ret != CL_SUCCESS) {
        auto ec = std::error_code(ret, std::generic_category());
        throw std::system_error(ec, "Could not create OpenCL program");
    }
    ret = clBuildProgram(program, 1, &clContext.deviceId, NULL, NULL, NULL);
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

void CLExecutable::runBlocking(const size_t globalWorkSize, const size_t localWorkSize) {
    cl_int ret = clEnqueueNDRangeKernel(context.defaultQueue.commandQueue, kernel, 1, NULL,
        &globalWorkSize, &localWorkSize, 0, NULL, NULL);
    if (ret != CL_SUCCESS) {
        auto ec = std::error_code(ret, std::generic_category());
        throw std::system_error(ec, "Could not run OpenCL kernel");
    }
    ret = clFinish(context.defaultQueue.commandQueue);
    if (ret != CL_SUCCESS) {
        auto ec = std::error_code(ret, std::generic_category());
        throw std::system_error(ec, "OpenCL kernel finished with error");
    }
}

CLExecutable::~CLExecutable() {
    clReleaseKernel(kernel);
    clReleaseProgram(program);
}

}