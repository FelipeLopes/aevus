#pragma once

#include "flame.hpp"
#include "mobius.hpp"
#include <memory>
#include <vector>

class KeyGasket {
public:
    KeyGasket();
    KeyGasket(std::vector<Mobius<double>> ifsTransforms, int diveIndex);
    double logscale;
    Flame toFlame(double logscale = 0, std::shared_ptr<Palette> palette = nullptr);
private:
    std::vector<Mobius<double>> ifsTransforms;
    int diveIndex = 0;
};