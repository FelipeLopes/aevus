#pragma once

#include "key_gasket.hpp"
#include <boost/gil.hpp>

class Colorer {
public:
    Colorer();
    Flame color(KeyGasket keyGasket, int zoomTransform);
private:
    static const boost::gil::rgb8_pixel_t RED, WHITE;
};