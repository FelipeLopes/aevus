#pragma once

#include <vector>
#include "../serial/xml_element.hpp"
#include "../serial/serializable.hpp"
#include "color.hpp"

namespace core {

class PaletteColors: public serial::StringSerializable {
public:
    PaletteColors();
    std::string hexAt(int pos) const;
    void readColorCLArray(std::vector<ColorCL>& arr) const;
    virtual std::optional<std::string> toString();
    virtual void fromString(std::optional<std::string> text);
    static const unsigned PALETTE_WIDTH = 256;
private:
    typedef unsigned char byte;
    char hexHigh(byte b) const;
    char hexLow(byte b) const;
    byte hexValue(char high, char low) const;
    std::vector<byte> paletteData;
};

class Palette: public serial::XMLElementClass {
public:
    Palette(XMLElementClass& el);
    serial::XMLAttributeInt count;
    serial::XMLAttributeString format;
    serial::XMLContent<PaletteColors> colors;
    void readColorCLArray(std::vector<ColorCL>& arr) const;
};

}