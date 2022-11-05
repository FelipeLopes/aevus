#pragma once

#include "colorer.hpp"
#include "flame.hpp"
#include "mobius.hpp"
#include <memory>
#include <vector>

class KeyGasket {
public:
    KeyGasket();
    KeyGasket(std::vector<Mobius<double>> ifsTransforms, double logscale);
    double logscale = 0;
    Flame toFlame(Colorer::ColorParams colorParams, double logscale = 0);
    int numTransforms();
private:
    std::vector<Mobius<double>> ifsTransforms;
};