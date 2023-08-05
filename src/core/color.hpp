#pragma once

#include "../serial/serializable.hpp"

namespace core {

struct ColorCL {
    float r,g,b,a;
};

class Color: public serial::StringSerializable {
public:
    Color();
    Color(uint8_t r, uint8_t g, uint8_t b);
    uint8_t r, g, b;
    ColorCL toColorCL();
    virtual std::optional<std::string> toString();
    virtual void fromString(std::optional<std::string> text);
};

}