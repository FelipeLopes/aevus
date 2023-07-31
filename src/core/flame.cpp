#include "flame.hpp"
#include "xml_serialization.hpp"
#include <algorithm>
#include <cctype>
#include <cstdint>
#include <cstdio>
#include <inttypes.h>
#include <iomanip>
#include <optional>
#include <random>
#include <stdexcept>
#include <string>

using std::map;
using std::optional;
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
    xforms(*this, "xform"),
    palette(*this) { }

FlameCL Flame::getFlameCL() const {
    FlameCL flameCL;
    flameCL.cx = center.getValue().x;
    flameCL.cy = center.getValue().y;
    flameCL.scale = scale.getValue();
    flameCL.width = size.getValue().width;
    flameCL.height = size.getValue().height;
    return flameCL;
}

void Flame::readInitialStateArray(std::vector<IterationState> &arr, int size) const {
    std::mt19937_64 rng(314159);
    std::uniform_int_distribution<uint64_t> seedDist;
    std::uniform_int_distribution<uint8_t> xformDist(0,xforms.size()-1);
    std::uniform_real_distribution<float> posDist(-1.0, 1.0);
    std::uniform_real_distribution<float> colorDist(0.0, 1.0);
    for (int i = 0; i < size; i++) {
        IterationState st;
        st.x = posDist(rng);
        st.y = posDist(rng);
        st.c = colorDist(rng);
        st.seed.value = seedDist(rng);
        st.xf = xformDist(rng);
        arr.push_back(st);
    }
}

void Flame::readXFormCLArray(std::vector<XFormCL>& arr) const {
    arr.resize(xforms.size());
    for (int i=0; i<arr.size(); i++) {
        arr[i] = xforms.get(i)->toXFormCL();
    }
}

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
        ans[it->second] = formattedDouble(kv.second);
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

optional<string> PaletteColors::toString() {
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

void PaletteColors::fromString(optional<string> text) {
    if (!text.has_value()) {
        throw std::invalid_argument("Palette color string does not exist");
    }
    auto str = text.value();
    str.erase(std::remove_if(str.begin(), str.end(), isspace), str.end());
    if (str.size() != PALETTE_WIDTH * 6) {
        throw std::invalid_argument("Palette color string has wrong size");
    }
    paletteData.resize(PALETTE_WIDTH * 3);
    for (int i=0; i<PALETTE_WIDTH; i++) {
        paletteData[3*i] = hexValue(str[6*i], str[6*i+1]);
        paletteData[3*i+1] = hexValue(str[6*i+2], str[6*i+3]);
        paletteData[3*i+2] = hexValue(str[6*i+4], str[6*i+5]);
    }
}

Affine::Affine(bool serializeIdentity_): serializeIdentity(serializeIdentity_) {
    xx = yy = 1;
    xy = yx = ox = oy = 0;
 }

optional<string> Affine::toString() {
    string ans = formattedDouble(xx) + " " +
        formattedDouble(-xy) + " " +
        formattedDouble(-yx) + " " +
        formattedDouble(yy) + " " +
        formattedDouble(ox) + " " +
        formattedDouble(-oy);
    if (ans == "1 0 0 1 0 0" && !serializeIdentity) {
        return std::nullopt;
    }
    return ans;
}

void Affine::fromString(optional<string> text) {
    if (!text.has_value()) {
        return;
    }
    auto str = text.value();
    if (sscanf(str.c_str(), "%lf %lf %lf %lf %lf %lf",
        &xx, &xy, &yx, &yy, &ox, &oy) < 6)
    {
        throw std::invalid_argument("Could not read Affine");
    }
    xy *= -1;
    yx *= -1;
    oy *= -1;
}

CoefsAffine::CoefsAffine(): Affine(true) { }

PostAffine::PostAffine(): Affine(false) { }

Chaos::Chaos() { }

double Chaos::getXaos(int idx) {
    if (idx < 0 || idx >= chaos.size()) {
        return 1.0;
    } else {
        return chaos[idx];
    }
}

optional<string> Chaos::toString() {
    string ans;
    for (int i=0; i<chaos.size(); i++) {
        if (i>0) {
            ans += " ";
        }
        ans += formattedDouble(chaos[i]);
    }
    if (ans == "") {
        return std::nullopt;
    }
    return ans;
}

void Chaos::fromString(optional<string> text) {
    if (!text.has_value()) {
        return;
    }
    auto str = text.value();
    FILE* stream = fmemopen((void*)str.c_str(), str.size(), "r");
    double val;
    while (fscanf(stream, " %lf", &val) > 0) {
        chaos.push_back(val);
    }
    fclose(stream);
}

ColorSpeed::ColorSpeed() { }

void ColorSpeed::fromStringMap(map<string, string> stringMap) {
    if (stringMap.find("color_speed") != stringMap.end()) {
        colorSpeed = std::stod(stringMap["color_speed"]);
    } else if (stringMap.find("symmetry") != stringMap.end()) {
        colorSpeed = std::stod(stringMap["symmetry"]);
    } else {
        colorSpeed = 0.5;
    }
}

map<string, string> ColorSpeed::toStringMap() {
    return boost::assign::map_list_of("color_speed", formattedDouble(colorSpeed));
}

XForm::XForm(): XMLElementClass("xform"),
    weight(*this, "weight"),
    color(*this, "color"),
    variationMap(*this, [](auto& names) {
        for (auto kv: Variation::variationNames.right) {
            names.insert(kv.first);
        }
    }),
    coefs(*this, "coefs"),
    post(*this, "post"),
    chaos(*this, "chaos"),
    colorSpeed(*this, {"color_speed", "symmetry"}) { }

XFormCL XForm::toXFormCL() const {
    XFormCL xf;

    auto pre = coefs.getValue();

    xf.a = pre.xx;
    xf.b = -pre.yx;
    xf.c = pre.ox;
    xf.d = -pre.xy;
    xf.e = pre.yy;
    xf.f = -pre.oy;

    auto pst = post.getValue();

    xf.pa = pst.xx;
    xf.pb = -pst.yx;
    xf.pc = pst.ox;
    xf.pd = -pst.xy;
    xf.pe = pst.yy;
    xf.pf = -pst.oy;

    int sz = 0;
    for (auto kv: variationMap.getValue().variations) {
        xf.varData[sz].id = kv.first;
        xf.varData[sz].weight = kv.second;
        sz++;
    }
    xf.varData[sz].id = Variation::NO_VARIATION;

    xf.color = color.getValue();
    xf.colorSpeed = colorSpeed.getValue().colorSpeed;

    return xf;
}

void Flame::readXFormDistribution(std::vector<uint8_t>& dist) const {
    dist.resize(XFORM_DISTRIBUTION_GRAINS*xforms.size());
    for (int i=0; i<xforms.size(); i++) {
        double acc = 0;
        std::vector<double> densities;
        for (int j=0; j<xforms.size(); j++) {
            acc += xforms.get(i)->chaos.getValue().getXaos(j) *
                xforms.get(j)->weight.getValue();
            densities.push_back(acc);
        }
        double step = acc / XFORM_DISTRIBUTION_GRAINS;
        int j = 0;
        double curr = 0;
        for (int k=0; k<XFORM_DISTRIBUTION_GRAINS; k++) {
            while (curr >= densities[j] && j<xforms.size()) {
                j++;
            }
            dist[i*XFORM_DISTRIBUTION_GRAINS+k] = j;
            curr += step;
        }
    }
}

Palette::Palette(XMLElementClass& el): XMLElementClass(el, "palette"),
    count(*this, "count"),
    format(*this, "format"),
    colors(*this)
{
    count.setValue(256);
    format.setValue("RGB");
}

void Palette::readColorCLArray(vector<ColorCL>& arr) const {
    colors.getValue().readColorCLArray(arr);
}

SizeParams::SizeParams(): SizeParams(0,0) { }

SizeParams::SizeParams(int width_, int height_):
    width(width_), height(height_) { }

optional<string> SizeParams::toString() {
    string ans = formattedDouble(width) + " " + formattedDouble(height);
    return ans;
}

void SizeParams::fromString(optional<string> text) {
    if (!text.has_value()) {
        throw std::invalid_argument("SizeParams string does not exist");
    }
    auto str = text.value();
    if (sscanf(str.c_str(),"%d %d", &width, &height) < 2) {
        throw std::invalid_argument("Could not read SizeParams");
    }
}

CenterParams::CenterParams(): CenterParams(0.0, 0.0) { }

CenterParams::CenterParams(double x_, double y_):
    x(x_), y(y_) { }

optional<string> CenterParams::toString() {
    string ans = formattedDouble(x) + " " + formattedDouble(y);
    return ans;
}

void CenterParams::fromString(optional<string> text) {
    if (!text.has_value()) {
        throw std::invalid_argument("CenterParams string does not exist");
    }
    auto str = text.value();
    if (sscanf(str.c_str(),"%lf %lf", &x, &y) < 2) {
        throw std::invalid_argument("Could not read CenterParams");
    }
}

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