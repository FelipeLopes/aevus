#pragma once

#include "flame.h"
#include "mobius.h"
#include <memory>
#include <vector>

class KeyGasket {
public:
    std::vector<Mobius<double>> ifsTransforms;
    double logscale;
    Flame toFlame(double logscale = 0, std::shared_ptr<Palette> palette = nullptr);
};