#pragma once

#include <boost/signals2.hpp>

namespace ui {

class EventBroker {
public:
    EventBroker();

    boost::signals2::signal<void ()> activeXformValueChanged;

    void preTransformValueChanged();
    void postTransformValueChanged();
};

}