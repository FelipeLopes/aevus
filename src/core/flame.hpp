#pragma once

#include "xml_serialization.hpp"

namespace core {

class Palette : public XMLElementClass {
public:
    Palette(XMLElementClass& el);
    XMLAttributeInt count;
    XMLAttributeString format;
    XMLContentString colors;
};

class Flame : public XMLElementClass {
public:
    Flame();
    XMLAttributeString version;
    XMLAttributeDouble scale;
    XMLAttributeInt quality;
    XMLAttributeDouble brightness;
    XMLAttributeDouble contrast;
    Palette palette;
};

}