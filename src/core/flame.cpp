#include "flame.hpp"
#include "xml_serialization.hpp"
#include <cstdint>
#include <inttypes.h>
#include <stdexcept>
#include <string>

using std::string;
using std::to_string;

namespace core {

Flame::Flame(): XMLElementClass("flame"),
    version(*this, "version"),
    name(*this, "name"),
    size(*this, "size"),
    center(*this, "center"),
    scale(*this, "scale"),
    quality(*this, "quality"),
    background(*this, "background"),
    brightness(*this, "brightness"),
    contrast(*this, "contrast", 1.0),
    initial(*this, "initial", 20),
    xform(*this),
    palette(*this) { }

XForm::XForm(XMLElementClass& el): XMLElementClass(el, "xform"),
    weight(*this, "weight"),
    color(*this, "color"),
    opacity(*this, "opacity") { }

Palette::Palette(XMLElementClass& el): XMLElementClass(el, "palette"),
    count(*this, "count"),
    format(*this, "format"),
    colors(*this)
{
    count.setValue(256);
    format.setValue("RGB");
}

SizeParams::SizeParams(): SizeParams(0,0) { }

SizeParams::SizeParams(int width_, int height_):
    width(width_), height(height_) { }

string SizeParams::toString() {
    string ans = to_string(width) + " " + to_string(height);
    return ans;
}

void SizeParams::fromString(string text) {
    if (sscanf(text.c_str(),"%d %d", &width, &height) < 2) {
        throw std::invalid_argument("Could not read SizeParams");
    }
}

CenterParams::CenterParams(): CenterParams(0.0, 0.0) { }

CenterParams::CenterParams(double x_, double y_):
    x(x_), y(y_) { }

string CenterParams::toString() {
    string ans = to_string(x) + " " + to_string(y);
    return ans;
}

void CenterParams::fromString(string text) {
    if (sscanf(text.c_str(),"%lf %lf", &x, &y) < 2) {
        throw std::invalid_argument("Could not read CenterParams");
    }
}

Color::Color(): Color(0,0,0) { }

Color::Color(uint8_t r_, uint8_t g_, uint8_t b_):
    r(r_), g(g_), b(b_) { }

string Color::toString() {
    string ans = to_string(r) + " " + to_string(g) + " " + to_string(b);
    return ans;
}

void Color::fromString(string text) {
    if (sscanf(text.c_str(),"%" SCNu8 " %" SCNu8 " %" SCNu8, &r, &g, &b) < 3) {
        throw std::invalid_argument("Could not read Color");
    }
}

}