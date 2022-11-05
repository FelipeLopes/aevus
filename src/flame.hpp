#pragma once

#include "palette.hpp"
#include "xform.hpp"
#include <memory>

class Flame {
public:
    Flame(Palette palette);
    tinyxml2::XMLNode* toXMLNode(tinyxml2::XMLDocument& xmlDoc);
    std::vector<XForm> xforms;
    const Palette palette;
};