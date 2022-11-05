#pragma once

#include "palette.hpp"
#include <boost/gil.hpp>

class Colorer {
public:
    Colorer();
    struct ColorParams {
        Palette palette;
        std::vector<double> colorValues;
    };
    ColorParams color(int numTransforms, int diveTransform, double logscale,
        double iniKeyLogscale, double endKeyLogscale) const;
    Palette palette;
private:
    static const boost::gil::rgb8_pixel_t RED, WHITE;
};