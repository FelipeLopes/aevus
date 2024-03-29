#pragma once

#include "cl_context.hpp"
#include <CL/cl.h>
#include <vector>

namespace clwrap {

class OpenCL {
public:
    static OpenCL* getInstance() {
        static OpenCL instance;
        return &instance;
    }
    CLContext createContext(int platformIdx, int deviceIdx);
    CLQueuedContext createQueuedContext(int platformIdx, int deviceIdx);
private:
    OpenCL();
    OpenCL(OpenCL const&) = delete;
    void operator= (OpenCL const&) = delete;
    std::vector<cl_platform_id> platformIDs;
    std::vector<std::vector<cl_device_id>> deviceIDs;
};

}