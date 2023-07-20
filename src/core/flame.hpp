#pragma once

#include "xml_serialization.hpp"
#include <cstdint>

namespace core {

class Palette: public XMLElementClass {
public:
    Palette(XMLElementClass& el);
    XMLAttributeInt count;
    XMLAttributeString format;
    XMLContentString colors;
};

class SizeParams: public StringSerializable {
public:
    SizeParams();
    SizeParams(int width, int height);
    int width, height;
    virtual std::string toString();
    virtual void fromString(std::string text);
};

class CenterParams: public StringSerializable {
public:
    CenterParams();
    CenterParams(double x, double y);
    double x, y;
    virtual std::string toString();
    virtual void fromString(std::string text);
};

class XForm: public XMLElementClass {
public:
    XForm(XMLElementClass& el);
    XMLAttributeDouble weight;
    XMLAttributeDouble color;
    XMLAttributeDouble opacity;
};

class Color: public StringSerializable {
public:
    Color();
    Color(uint8_t r, uint8_t g, uint8_t b);
    uint8_t r, g, b;
    virtual std::string toString();
    virtual void fromString(std::string text);
};

class Flame: public XMLElementClass {
public:
    Flame();
    XMLAttributeString version;
    XMLAttributeString name;
    XMLAttribute<SizeParams> size;
    XMLAttribute<CenterParams> center;
    XMLAttributeDouble scale;
    XMLAttributeInt quality;
    XMLAttribute<Color> background;
    XMLAttributeDouble brightness;
    XMLAttributeDouble contrast;
    XMLAttributeInt initial;
    XForm xform;
    Palette palette;
};

}