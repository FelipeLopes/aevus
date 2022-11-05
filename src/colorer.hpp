#pragma once

#include "key_gasket.hpp"
#include <boost/gil.hpp>

struct ColorParams {
    Palette palette;
    std::vector<double> colorValues;
};

class Colorer {
public:
    Colorer();
    ColorParams color(int numTransforms, int diveTransform, double logscale,
        double iniKeyLogscale, double endKeyLogscale);
    Palette palette;
private:
    static const boost::gil::rgb8_pixel_t RED, WHITE;
};