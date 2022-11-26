#pragma once

#include "palette.hpp"
#include "../render/flame_cl.hpp"
#include "../render/xform.hpp"
#include "../render/xform_distribution.hpp"
#include <memory>

namespace render {

class Flame {
public:
    Flame(double centerX, double centerY, int width, int height, Palette palette);
    tinyxml2::XMLNode* toXMLNode(tinyxml2::XMLDocument& xmlDoc);
    std::vector<render::XForm> xforms;
    void readXFormCLArray(std::vector<render::XFormCL>& arr) const;
    void readXFormDistribution(render::XFormDistribution& dist) const;
    render::FlameCL getFlameCL() const;
    const Palette palette;
private:
    double centerX, centerY, scale;
    int width, height;
};

}