#pragma once

#include "colorer.hpp"
#include "flame.hpp"
#include "mobius.hpp"
#include <memory>
#include <vector>

class KeyGasket {
public:
    KeyGasket(std::vector<Mobius<double>> ifsTransforms, int level);
    int level = 0;
    Flame toFlame(Colorer::ColorParams colorParams, double logscale = 0) const;
    int numTransforms() const;
private:
    std::vector<Mobius<double>> ifsTransforms;
};