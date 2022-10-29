#pragma once

#include "palette.h"
#include "xform.h"

class Flame {
public:
    Flame();
    tinyxml2::XMLNode* toXMLNode(tinyxml2::XMLDocument& xmlDoc);
    std::vector<XForm> xforms;
    Palette palette;
};