#pragma once

#include "xml_serialization.hpp"
#include <cstdint>
#include <boost/bimap.hpp>

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

struct Variation {
    enum VariationID {
        NO_VARIATION = -1,
        LINEAR = 0,
        SPHERICAL = 2,
        POLAR = 6,
        HYPERBOLIC = 11,
        DIAMOND = 12,
        EYEFISH = 28,
        CYLINDER = 30,
        SQUARE = 44
    };

    const static boost::bimap<VariationID, std::string> variationNames;
};

class VariationMap: public StringMapSerializable {
public:
    VariationMap();
    virtual std::map<std::string, std::string> toStringMap();
    virtual void fromStringMap(std::map<std::string, std::string> stringMap);
    std::map<Variation::VariationID, double> variations;
};

class XForm: public XMLElementClass {
public:
    XForm();
    XMLAttributeDouble weight;
    XMLAttributeDouble color;
    XMLMultiAttribute<VariationMap> variationMap;
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
    ListXMLElementClass<XForm> xforms;
    Palette palette;
};

}