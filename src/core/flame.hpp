#pragma once

#include "xml_serialization.hpp"

namespace core {

class Palette: public XMLElementClass {
public:
    Palette(XMLElementClass& el);
    XMLAttributeInt count;
    XMLAttributeString format;
    XMLContentString colors;
};

class SizeParams: public XMLSerializable {
public:
    SizeParams();
    int width, height;
    virtual std::string toXMLString();
    virtual void fromXMLString(std::string text);
};

class Flame: public XMLElementClass {
public:
    Flame();
    XMLAttributeString name;
    XMLAttributeString version;
    XMLAttribute<SizeParams> size;
    XMLAttributeDouble scale;
    XMLAttributeInt quality;
    XMLAttributeDouble brightness;
    XMLAttributeDouble contrast;
    Palette palette;
};

}