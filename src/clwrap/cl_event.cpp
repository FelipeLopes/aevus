#include "cl_event.hpp"
#include <CL/cl.h>
#include <stdexcept>

namespace clwrap {

CLEvent::CLEvent(cl_event clEvent_): clEvent(clEvent_) { }

CLEvent::~CLEvent() {
    clReleaseEvent(clEvent);
}

CLEventCallback::CLEventCallback(std::shared_ptr<CLEvent> event_, CLEvent::Status status_,
    std::function<void()> f_): event(event_), status(status_), f(f_) { }

}