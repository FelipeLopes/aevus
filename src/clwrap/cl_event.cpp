#include "cl_event.hpp"
#include <CL/cl.h>
#include <stdexcept>

namespace clwrap {

CLEvent::CLEvent(cl_event clEvent_): clEvent(clEvent_) { }

CLEvent::~CLEvent() {
    clReleaseEvent(clEvent);
}

cl_int CLEvent::convertStatus(Status status) {
    switch (status) {
        case SUBMITTED: return CL_SUBMITTED;
        case RUNNING: return CL_RUNNING;
        case COMPLETE: return CL_COMPLETE;
    }
    throw std::invalid_argument("Unrecognized event execution status");
}

}