#include "flame.hpp"
#include "xml_serialization.hpp"
#include <algorithm>
#include <cctype>
#include <cstdint>
#include <inttypes.h>
#include <iomanip>
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

char PaletteColors::hexHigh(byte b) const {
    return b/16 > 9 ? (b/16)+'A'-10 : (b/16)+'0';
}

char PaletteColors::hexLow(byte b) const {
    return b%16 > 9 ? (b%16)+'A'-10 : (b%16)+'0';
}

PaletteColors::byte PaletteColors::hexValue(char high, char low) const {
    return (high >= 'A' ? (10 + high - 'A') : (high - '0')) * 16
        + (low >= 'A' ? (10 + low - 'A') : (low - '0'));
}

string PaletteColors::hexAt(int pos) const {
    char s[7];
    byte r = paletteData[3*pos];
    byte g = paletteData[3*pos+1];
    byte b = paletteData[3*pos+2];
    s[0] = hexHigh(r);
    s[1] = hexLow(r);
    s[2] = hexHigh(g);
    s[3] = hexLow(g);
    s[4] = hexHigh(b);
    s[5] = hexLow(b);
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
    string whiteSpace(8, ' ');
    string text = "\n";
    for (int i=0; i<32; i++) {
        string paletteChars = "";
        for (int j=0; j<8; j++) {
            paletteChars += hexAt(i*8+j);
        }
        text += (whiteSpace + paletteChars + "\n");
    }
    text += "    ";
    return text;
}

void PaletteColors::fromString(string text) {
    text.erase(std::remove_if(text.begin(), text.end(), isspace), text.end());
    if (text.size() != PALETTE_WIDTH * 6) {
        throw std::invalid_argument("Palette color string has wrong size");
    }
    paletteData.resize(PALETTE_WIDTH * 3);
    for (int i=0; i<PALETTE_WIDTH; i++) {
        paletteData[3*i] = hexValue(text[6*i], text[6*i+1]);
        paletteData[3*i+1] = hexValue(text[6*i+2], text[6*i+3]);
        paletteData[3*i+2] = hexValue(text[6*i+4], text[6*i+5]);
    }
}

Affine::Affine() {
    ox = oy = 0;
    xx = yy = 1;
    xy = yx = 0;
}

Affine::Affine(double xx_, double xy_, double yx_, double yy_, double ox_, double oy_):
    xx(xx_), xy(xy_), yx(yx_), yy(yy_), ox(ox_), oy(oy_) { }

string Affine::toString() {
    std::stringstream buffer;
    buffer<<std::fixed<<std::setprecision(6)<<
        xx<<" "<<-xy<<" "<<
        -yx<<" "<<yy<<" "<<
        ox<<" "<<-oy;
    return buffer.str();
}

void Affine::fromString(string text) {
    if (sscanf(text.c_str(), "%lf %lf %lf %lf %lf %lf",
        &xx, &xy, &yx, &yy, &ox, &oy) < 6)
    {
        throw std::invalid_argument("Could not read Affine");
    }
    xy *= -1;
    yx *= -1;
    oy *= -1;
}

XForm::XForm(): XMLElementClass("xform"),
    weight(*this, "weight"),
    color(*this, "color"),
    variationMap(*this, [](auto& names) {
        for (auto kv: Variation::variationNames.right) {
            names.insert(kv.first);
        }
    }),
    coefs(*this, "coefs", true),
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