#pragma once

#include "color_params.hpp"
#include "key_gasket.hpp"
#include <algorithm>
#include <boost/gil.hpp>

class Colorer {

public:
    Colorer() { }
    virtual ColorParams color(const std::map<double, KeyGasket>& keyGaskets,
        double logscale, int diveTransform) const = 0;
    virtual ~Colorer() { }
};