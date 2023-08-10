#include "custom_coefs.hpp"
#include <boost/assign.hpp>
#include <optional>
#include <stdexcept>

using std::map;
using std::optional;
using std::stod;
using std::string;

namespace core {

SizeParams::SizeParams(): SizeParams(0,0) { }

SizeParams::SizeParams(int width_, int height_):
    width(width_), height(height_) { }

optional<string> SizeParams::toString() {
    string ans = serial::formattedDouble(width) + " " + serial::formattedDouble(height);
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
    string ans = serial::formattedDouble(x) + " " + serial::formattedDouble(y);
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

Affine::Affine(bool serializeIdentity_): serializeIdentity(serializeIdentity_) {
    xx = yy = 1;
    xy = yx = ox = oy = 0;
}

optional<string> Affine::toString() {
    string ans = serial::formattedDouble(xx) + " " +
        serial::formattedDouble(-xy) + " " +
        serial::formattedDouble(-yx) + " " +
        serial::formattedDouble(yy) + " " +
        serial::formattedDouble(ox) + " " +
        serial::formattedDouble(-oy);
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

double Affine::getValueByIndex(int idx) {
    switch (idx) {
        case 0: return xx; break;
        case 1: return xy; break;
        case 2: return yx; break;
        case 3: return yy; break;
        case 4: return ox; break;
        case 5: return oy; break;
        default: throw std::invalid_argument("Unrecognized index");
    }
}

void Affine::setValueByIndex(int idx, double val) {
    switch (idx) {
        case 0: xx = val; break;
        case 1: xy = val; break;
        case 2: yx = val; break;
        case 3: yy = val; break;
        case 4: ox = val; break;
        case 5: oy = val; break;
        default: throw std::invalid_argument("Unrecognized index");
    }
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
        ans += serial::formattedDouble(chaos[i]);
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
        colorSpeed = stod(stringMap["color_speed"]);
    } else if (stringMap.find("symmetry") != stringMap.end()) {
        colorSpeed = stod(stringMap["symmetry"]);
    } else {
        colorSpeed = 0.5;
    }
}

map<string, string> ColorSpeed::toStringMap() {
    return boost::assign::map_list_of("color_speed", serial::formattedDouble(colorSpeed));
}

}