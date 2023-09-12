#pragma once

#include <CL/cl.h>
#include <CL/opencl.hpp>

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
    template <typename T>
    void setCallback(T* object, Status status, void (*f)(T* obj));
private:
    cl_event clEvent;
    cl_int convertStatus(Status status);
    void* object;
    void (*callback)(void* obj);
    static void clCallback(cl_event event, cl_int execStatus, void* thisObject) {
        CLEvent* eventObject = reinterpret_cast<CLEvent*>(thisObject);
        eventObject->callback(eventObject->object);
    }
};

template <typename T>
void CLEvent::setCallback(T* obj, Status status, void (*f)(T*)) {
    object = obj;
    callback = reinterpret_cast<void (*)(void*)>(f);
    clSetEventCallback(clEvent, convertStatus(status), clCallback, this);
}

}