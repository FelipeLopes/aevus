#include "event_broker.hpp"

namespace ui {

EventBroker::EventBroker() { }

void EventBroker::preTransformValueChanged() {
    activeXformValueChanged();
}

void EventBroker::postTransformValueChanged() {
    activeXformValueChanged();
}

}