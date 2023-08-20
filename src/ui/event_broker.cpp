#include "event_broker.hpp"

namespace ui {

EventBroker::EventBroker() { }

void EventBroker::preTransformValueChanged() {
    activeXformCoordsChanged();
}

void EventBroker::postTransformValueChanged() {
    activeXformCoordsChanged();
}

void EventBroker::xformSelected(int id) {
    activeXformChanged(id);
}

void EventBroker::weightValueChanged() {
    flameWeightsChanged();
}

void EventBroker::variationValueChanged() {
    variationParamsChanged();
}

}