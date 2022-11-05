#pragma once

#include "palette.hpp"
#include <algorithm>
#include <boost/gil.hpp>

class Colorer {

using rgb8_pixel_t = boost::gil::rgb8_pixel_t;

public:
    Colorer(): palette(WHITE, RED) {

    }
    struct ColorParams {
        Palette palette;
        std::vector<double> colorValues;
    };
    ColorParams color(int numTransforms, int diveTransform, double logscale,
        double iniKeyLogscale, double endKeyLogscale) const {

        ColorParams params;
        params.palette = palette;
        double f = (logscale-iniKeyLogscale)/(endKeyLogscale-iniKeyLogscale);
        double diveVal = std::min(1.0, 2*f);
        double nonDiveVal = std::max(0.0, 2*f-1);

        for (int i=0; i<numTransforms; i++) {
            double val = (i == diveTransform) ? diveVal : nonDiveVal;
            params.colorValues.push_back(val);
        }

        return params;
    }
    Palette palette;
private:
    static const boost::gil::rgb8_pixel_t RED, WHITE;
};