#pragma once

#include "flame.hpp"
#include "mobius.hpp"
#include <memory>
#include <vector>

class KeyGasket {
public:
    KeyGasket();
    KeyGasket(std::vector<Mobius<double>> ifsTransforms, double logscale, int diveIndex);
    double logscale = 0;
    Flame toFlame(Palette palette, double logscale = 0);
private:
    std::vector<Mobius<double>> ifsTransforms;
    int diveIndex = 0;
};