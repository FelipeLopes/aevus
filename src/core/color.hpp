#pragma once

#include <inttypes.h>
#include <optional>
#include <string>

namespace core {

struct ColorCL {
    float r,g,b,a;
    ColorCL() {
        r = g = b = 0;
        a = 1;
    }
};

class Color {
public:
    Color();
    Color(uint8_t r, uint8_t g, uint8_t b);
    uint8_t r, g, b;
    ColorCL toColorCL() const;
    std::optional<std::string> toString();
    void fromString(std::optional<std::string> text);
};

}