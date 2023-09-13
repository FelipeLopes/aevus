#pragma once

#include <CL/cl.h>
#include <CL/opencl.hpp>
#include <memory>

namespace clwrap {

class CLEvent {
public:
    CLEvent(cl_event clEvent);
    ~CLEvent();
    enum Status {
        SUBMITTED,
        RUNNING,
        COMPLETE
    };
    static CLEvent::Status convertCLStatus(cl_int status) {
        switch (status) {
            case CL_SUBMITTED: return SUBMITTED;
            case CL_RUNNING: return RUNNING;
            case CL_COMPLETE: return COMPLETE;
        }
        throw std::invalid_argument("Unrecognized event execution status");
    }
    static cl_int convertStatus(Status status) {
        switch (status) {
            case SUBMITTED: return CL_SUBMITTED;
            case RUNNING: return CL_RUNNING;
            case COMPLETE: return CL_COMPLETE;
        }
        throw std::invalid_argument("Unrecognized event execution status");
    }
    cl_event clEvent;
};

class CLEventCallback {
public:
    CLEventCallback(std::shared_ptr<CLEvent> event, CLEvent::Status status,
        std::function<void()> f);
    std::shared_ptr<CLEvent> event;
    CLEvent::Status status;
    std::function<void()> f;
};

}