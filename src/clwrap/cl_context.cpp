#include "cl_context.hpp"
#include "cl_buffer.hpp"
#include "cl_event.hpp"
#include "cl_executable.hpp"
#include "cl_queue.hpp"
#include <CL/cl.h>
#include <boost/asio/post.hpp>
#include <system_error>

namespace clwrap {

CLContext::CLContext(cl_device_id clDeviceId): deviceId(clDeviceId), callbackPool(1) {
    cl_int ret;
    context = clCreateContext(NULL, 1, &deviceId, NULL, NULL, &ret);
    if (ret != CL_SUCCESS) {
        auto ec = std::error_code(ret, std::generic_category());
        throw std::system_error(ec, "Could not create OpenCL context");
    }
}

CLQueue CLContext::createCommandQueue() {
    return CLQueue(context, deviceId);
}

void CLContext::setEventCallback(std::shared_ptr<CLEvent> event,
    CLEvent::Status status, std::function<void()> f)
{
    eventCallbacks.push_back(CLEventCallback(event, status, f));
    boost::asio::post(callbackPool, [event, status, this] {
        clSetEventCallback(event->clEvent, CLEvent::convertStatus(status), clCallback, this);
    });
}

CLContext::~CLContext() {
    clReleaseContext(context);
}

CLQueuedContext::CLQueuedContext(cl_device_id clDeviceId):
    CLContext(clDeviceId), defaultQueue(context, deviceId) { }

}