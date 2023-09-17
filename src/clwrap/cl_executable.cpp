#include "cl_executable.hpp"
#include "cl_arg.hpp"
#include "cl_context.hpp"
#include "cl_queue.hpp"
#include <CL/cl.h>
#include <fstream>
#include <sstream>

using std::vector;

namespace clwrap {

CLExecutable::CLExecutable(const CLQueuedContext& clContext, std::string name,
    const char* sourceStart, const char* sourceEnd):
    context(clContext)
{
    size_t sourceLen = sourceEnd - sourceStart;
    cl_int ret;
    program = clCreateProgramWithSource(clContext.context, 1, &sourceStart, &sourceLen, &ret);
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

std::shared_ptr<CLEvent> CLExecutable::runAsync(
    const size_t globalWorkSize, const size_t localWorkSize)
{
    cl_event event;
    vector<cl_event> depEvents;
    for (int i=0; i<bufferArgs.size(); i++) {
        if (bufferArgs[i]->isLazy()) {
            depEvents.push_back(bufferArgs[i]->lazySet()->clEvent);
        }
    }
    cl_int ret = clEnqueueNDRangeKernel(context.defaultQueue.commandQueue, kernel, 1, NULL,
        &globalWorkSize, &localWorkSize, depEvents.size(), depEvents.data(), &event);
    if (ret != CL_SUCCESS) {
        auto ec = std::error_code(ret, std::generic_category());
        throw std::system_error(ec, "Could not run OpenCL kernel");
    }
    return std::make_shared<CLEvent>(event);
}

CLExecutable::~CLExecutable() {
    clReleaseKernel(kernel);
    clReleaseProgram(program);
}

}