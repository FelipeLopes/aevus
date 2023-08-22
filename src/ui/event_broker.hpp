#pragma once

#include <boost/signals2.hpp>

namespace ui {

class EventBroker {
public:
    EventBroker();

    boost::signals2::signal<void ()> activeXformCoordsChanged;
    boost::signals2::signal<void (int)> activeXformChanged;
    boost::signals2::signal<void ()> flameWeightsChanged;
    boost::signals2::signal<void ()> variationParamsChanged;
    boost::signals2::signal<void ()> paletteChanged;
};

}