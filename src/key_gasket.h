#pragma once

#include "flame.h"
#include "mobius.h"
#include <vector>

class KeyGasket {
public:
    std::vector<Mobius<double>> ifsTransforms;
    double logscale;
    Flame toFlame();
};