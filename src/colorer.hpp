#pragma once

#include "palette.hpp"
#include <algorithm>
#include <boost/gil.hpp>

class Colorer {

public:
    Colorer() { }
    struct ColorParams {
        Palette palette;
        std::vector<double> colorValues;
    };
    virtual ColorParams color(int numTransforms, int diveTransform, double logscale,
        double iniKeyLogscale, double endKeyLogscale) const = 0;
    virtual ~Colorer() { }
};