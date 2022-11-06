#pragma once

#include "colorer.hpp"
#include "flame.hpp"
#include "mobius.hpp"
#include <memory>
#include <vector>

class KeyGasket {
public:
    KeyGasket();
    KeyGasket(std::vector<Mobius<double>> ifsTransforms, double logscale, int level = 0);
    double logscale = 0;
    int level = 0;
    Flame toFlame(Colorer::ColorParams colorParams, double logscale = 0) const;
    int numTransforms() const;
private:
    std::vector<Mobius<double>> ifsTransforms;
};