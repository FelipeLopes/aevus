#pragma once

#include "gradient.hpp"

namespace core {

class PresetLibrary {
public:
    PresetLibrary();
    std::vector<Gradient> gradients;
    bool addGrd5Gradient(const grad::Grd5SolidGradient& grd5Gradient);
};

}