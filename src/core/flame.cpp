#include "flame.hpp"
#include "xml_serialization.hpp"
#include <stdexcept>
#include <string>

using std::string;
using std::to_string;

namespace core {

Flame::Flame(): XMLElementClass("flame"),
    name(*this, "name"),
    version(*this, "version"),
    size(*this, "size"),
    center(*this, "center"),
    scale(*this, "scale"),
    quality(*this, "quality"),
    brightness(*this, "brightness"),
    contrast(*this, "contrast", 1.0),
    initial(*this, "initial", 20),
    palette(*this) { }

Palette::Palette(XMLElementClass& el): XMLElementClass(el, "palette"),
    count(*this, "count"),
    format(*this, "format"),
    colors(*this) { }

SizeParams::SizeParams() { }

string SizeParams::toXMLString() {
    string ans = to_string(width) + " " + to_string(height);
    return ans;
}

void SizeParams::fromXMLString(string text) {
    if (sscanf(text.c_str(),"%d %d", &width, &height) < 2) {
        throw std::invalid_argument("Could not read SizeParams");
    }
}

CenterParams::CenterParams() { }

string CenterParams::toXMLString() {
    string ans = to_string(x) + " " + to_string(y);
    return ans;
}

void CenterParams::fromXMLString(string text) {
    if (sscanf(text.c_str(),"%lf %lf", &x, &y) < 2) {
        throw std::invalid_argument("Could not read SizeParams");
    }
}

}