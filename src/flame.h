#pragma once

#include "palette.h"
#include "xform.h"
#include <memory>

class Flame {
public:
    Flame(std::shared_ptr<Palette> palette = nullptr);
    tinyxml2::XMLNode* toXMLNode(tinyxml2::XMLDocument& xmlDoc);
    std::vector<XForm> xforms;
    std::shared_ptr<Palette> palette;
};