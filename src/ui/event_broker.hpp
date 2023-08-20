#pragma once

#include <boost/signals2.hpp>

namespace ui {

class EventBroker {
public:
    EventBroker();

    boost::signals2::signal<void ()> activeXformCoordsChanged;
    boost::signals2::signal<void (int)> activeXformChanged;
    boost::signals2::signal<void ()> flameWeightsChanged;

    void preTransformValueChanged();
    void postTransformValueChanged();
    void xformSelectedOnWeights(int id);
    void weightValueChanged();
};

}