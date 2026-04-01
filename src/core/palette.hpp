#pragma once

#include <vector>
#include "serializable.hpp"
#include "color.hpp"

namespace core {

class PaletteColors {
public:
    PaletteColors();
    std::string hexAt(int pos) const;
    Color colorAt(int pos) const;
    void readColorCLArray(std::vector<ColorCL>& arr) const;
    virtual std::optional<std::string> toString();
    virtual void fromString(std::optional<std::string> text);
    typedef unsigned char byte;
    static const unsigned PALETTE_WIDTH = 256;
    std::vector<byte> paletteData;
private:
    char hexHigh(byte b) const;
    char hexLow(byte b) const;
    byte hexValue(char high, char low) const;
};

class Palette: public Serializable {
public:
    Palette();
    int count;
    std::string format;
    PaletteColors colors;
    void readColorCLArray(std::vector<ColorCL>& arr) const;
    virtual void acceptSerializer(Serializer& serializer);
    virtual void acceptDeserializer(Deserializer& deserializer);
};

}