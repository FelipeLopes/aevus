#include "opencl.hpp"
#include "cl_context.hpp"
#include "CL/cl.h"
#include <system_error>

namespace clwrap {

OpenCL::OpenCL() {
    cl_uint numPlatforms;
    cl_int ret = clGetPlatformIDs(0, NULL, &numPlatforms);
    if (ret != CL_SUCCESS) {
        auto ec = std::error_code(ret, std::generic_category());
        throw std::system_error(ec, "Could not get number of OpenCL platforms.");
    }
    platformIDs.resize(numPlatforms);
    deviceIDs.resize(numPlatforms);
    ret = clGetPlatformIDs(1, platformIDs.data(), NULL);
    if (ret != CL_SUCCESS) {
        auto ec = std::error_code(ret, std::generic_category());
        throw std::system_error(ec, "Could not get OpenCL platform IDs.");
    }
    for (int i = 0; i < numPlatforms; i++) {
        cl_uint numDevices;
        ret = clGetDeviceIDs(platformIDs[i], CL_DEVICE_TYPE_ALL, 0, NULL, &numDevices);
        if (ret != CL_SUCCESS) {
            auto ec = std::error_code(ret, std::generic_category());
            throw std::system_error(ec, "Could not get number of device IDs for OpenCL platform");
        }
        deviceIDs[i].resize(numDevices);
        ret = clGetDeviceIDs(platformIDs[i], CL_DEVICE_TYPE_ALL, numDevices,
            deviceIDs[i].data(), NULL);
        if (ret != CL_SUCCESS) {
            auto ec = std::error_code(ret, std::generic_category());
            throw std::system_error(ec, "Could not get device IDs for OpenCL platform");
        }
    }
}

CLContext OpenCL::createContext(int platformIdx, int deviceIdx) {
    return CLContext(deviceIDs[platformIdx][deviceIdx]);
}

CLQueuedContext OpenCL::createQueuedContext(int platformIdx, int deviceIdx) {
    return CLQueuedContext(deviceIDs[platformIdx][deviceIdx]);
}

}