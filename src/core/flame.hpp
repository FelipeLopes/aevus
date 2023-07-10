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

class SizeParams: public XMLSerializable {
public:
    SizeParams();
    SizeParams(int width, int height);
    int width, height;
    virtual std::string toXMLString();
    virtual void fromXMLString(std::string text);
};

class CenterParams: public XMLSerializable {
public:
    CenterParams();
    CenterParams(double x, double y);
    double x, y;
    virtual std::string toXMLString();
    virtual void fromXMLString(std::string text);
};

class Color: public XMLSerializable {
public:
    Color();
    Color(uint8_t r, uint8_t g, uint8_t b);
    uint8_t r, g, b;
    virtual std::string toXMLString();
    virtual void fromXMLString(std::string text);
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
    Palette palette;
};

}