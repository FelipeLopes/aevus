#pragma once

#include <CL/cl.h>

class CLContext {
public:
    CLContext(cl_device_id clDeviceId);
    ~CLContext();
private:
    cl_context context;
};