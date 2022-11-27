#pragma once

#include "palette.hpp"
#include "../render/flame_cl.hpp"
#include "../render/iteration_state.hpp"
#include "../render/xform.hpp"
#include <memory>

namespace render {

class Flame {
public:
    Flame(double centerX, double centerY, int width, int height, Palette palette);
    tinyxml2::XMLNode* toXMLNode(tinyxml2::XMLDocument& xmlDoc);
    std::vector<XForm> xforms;
    void readInitialStateArray(std::vector<IterationState>& arr) const;
    void readXFormCLArray(std::vector<XFormCL>& arr) const;
    void readXFormDistribution(std::vector<uint8_t>& dist) const;
    FlameCL getFlameCL() const;
    const Palette palette;
    const static int XFORM_DISTRIBUTION_GRAINS = 16384;
private:
    double centerX, centerY, scale;
    int width, height;
};

}