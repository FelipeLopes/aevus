#include "color.hpp"
#include <inttypes.h>
#include <optional>

using std::optional;
using std::string;
using std::to_string;

namespace core {

Color::Color(): Color(0,0,0) { }

Color::Color(uint8_t r_, uint8_t g_, uint8_t b_):
    r(r_), g(g_), b(b_) { }

ColorCL Color::toColorCL() {
    ColorCL ans;
    ans.r = r / 255.0;
    ans.g = g / 255.0;
    ans.b = b / 255.0;
    ans.a = 1.0;
    return ans;
}

optional<string> Color::toString() {
    string ans = to_string(r) + " " + to_string(g) + " " + to_string(b);
    return ans;
}

void Color::fromString(optional<string> text) {
    if (!text.has_value()) {
        throw std::invalid_argument("Color string does not exist");
    }
    auto str = text.value();
    if (sscanf(str.c_str(),"%" SCNu8 " %" SCNu8 " %" SCNu8, &r, &g, &b) < 3) {
        throw std::invalid_argument("Could not read Color");
    }
}

}