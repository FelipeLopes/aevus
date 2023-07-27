#include "flame.hpp"
#include "xml_serialization.hpp"
#include <cstdint>
#include <inttypes.h>
#include <stdexcept>
#include <string>

using std::map;
using std::string;
using std::stod;
using std::to_string;
using std::vector;
using boost::bimap;
using boost::assign::list_of;

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
    xforms(*this, "xform"),
    palette(*this) { }

const bimap<Variation::VariationID, string> Variation::variationNames =
    list_of<bimap<Variation::VariationID, string>::relation>
        (LINEAR, "linear")
        (SPHERICAL, "spherical")
        (POLAR, "polar")
        (HYPERBOLIC, "hyperbolic")
        (DIAMOND, "diamond")
        (EYEFISH, "eyefish")
        (CYLINDER, "cylinder")
        (SQUARE, "square");

VariationMap::VariationMap() { }

map<string, string> VariationMap::toStringMap() {
    map<string, string> ans;
    for (auto kv: variations) {
        auto it = Variation::variationNames.left.find(kv.first);
        if (it == Variation::variationNames.left.end()) {
            throw std::invalid_argument("Unknown variation ID");
        }
        ans[it->second] = to_string(kv.second);
    }
    return ans;
}

void VariationMap::fromStringMap(map<string, string> stringMap) {
    variations.clear();
    for (auto kv: stringMap) {
        auto it = Variation::variationNames.right.find(kv.first);
        if (it == Variation::variationNames.right.end()) {
            throw std::invalid_argument("Unknown variation name");
        }
        variations[it->second] = stod(kv.second);
    }
}

PaletteColors::PaletteColors() { }

string PaletteColors::hexAt(int pos) const {
    char s[7];
    byte r = paletteData[3*pos];
    byte g = paletteData[3*pos+1];
    byte b = paletteData[3*pos+2];
    s[0] = r/16 > 9 ? (r/16)+'A'-10 : (r/16)+'0';
    s[1] = r%16 > 9 ? (r%16)+'A'-10 : (r%16)+'0';
    s[2] = g/16 > 9 ? (g/16)+'A'-10 : (g/16)+'0';
    s[3] = g%16 > 9 ? (g%16)+'A'-10 : (g%16)+'0';
    s[4] = b/16 > 9 ? (b/16)+'A'-10 : (b/16)+'0';
    s[5] = b%16 > 9 ? (b%16)+'A'-10 : (b%16)+'0';
    s[6] = '\0';
    return std::string(s);
}

void PaletteColors::readColorCLArray(vector<ColorCL>& arr) const {
    arr.resize(PALETTE_WIDTH);
    for (int i=0; i<PALETTE_WIDTH; i++) {
        arr[i].r = paletteData[3*i] / 255.0;
        arr[i].g = paletteData[3*i+1] / 255.0;
        arr[i].b = paletteData[3*i+2] / 255.0;
        arr[i].a = 1.0;
    }
}

string PaletteColors::toString() {
    return "";
}

void PaletteColors::fromString(string text) {

}

XForm::XForm(): XMLElementClass("xform"),
    weight(*this, "weight"),
    color(*this, "color"),
    variationMap(*this, [](auto& names) {
        for (auto kv: Variation::variationNames.right) {
            names.insert(kv.first);
        }
    }),
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