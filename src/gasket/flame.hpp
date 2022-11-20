#pragma once

#include "palette.hpp"
#include "../render/xform.hpp"
#include <memory>

namespace gasket {

class Flame {
public:
    Flame(int width, int height, Palette palette);
    tinyxml2::XMLNode* toXMLNode(tinyxml2::XMLDocument& xmlDoc);
    std::vector<render::XForm> xforms;
    const Palette palette;
private:
    int width, height;
};

}