#include "gradient.hpp"
#include "serializable.hpp"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <memory>
#include <regex>
#include <stdexcept>
#include <string>
#include <tinyxml2.h>

using grad::Grd5SolidGradient;
using std::to_string;
using tinyxml2::XMLElement;
using tinyxml2::XMLNode;

namespace core {

GradientColor::GradientColor(double r_, double g_, double b_): r(r_), g(g_), b(b_) { }

GradientColor::GradientColor(): GradientColor(0.0, 0.0, 0.0) { }

// Code adapted from https://stackoverflow.com/a/6930407
GradientColor::HsvCoordinates GradientColor::toHsv() {
    HsvCoordinates out;
    double      min, max, delta;

    min = r < g ? r : g;
    min = min  < b ? min  : b;

    max = r > g ? r : g;
    max = max  > b ? max  : b;

    out.v = max;                                // v
    delta = max - min;
    if (delta < 0.00001) {
        out.s = 0;
        out.h = 0; // undefined, maybe nan?
        return out;
    }
    if( max > 0.0 ) { // NOTE: if Max is == 0, this divide would cause a crash
        out.s = (delta / max);                  // s
    } else {
        // if max is 0, then r = g = b = 0              
        // s = 0, h is undefined
        out.s = 0.0;
        out.h = NAN;                            // its now undefined
        return out;
    }
    if( r >= max ) {                    // > is bogus, just keeps compilor happy
        out.h = ( g - b ) / delta;
    }        // between yellow & magenta
    else if( g >= max ) {
        out.h = 2.0 + ( b - r ) / delta;  // between cyan & yellow
    }
    else {
        out.h = 4.0 + ( r - g ) / delta;  // between magenta & cyan
    }
    out.h *= 60.0;                              // degrees

    if( out.h < 0.0 )
        out.h += 360.0;

    out.s *= 100;
    out.v *= 100;

    return out;
}

// Code adapted from https://stackoverflow.com/a/6930407
GradientColor GradientColor::fromHsv(GradientColor::HsvCoordinates in) {
    double      hh, p, q, t, ff, r, g ,b;
    long        i;

    in.s /= 100;
    in.v /= 100;

    if(in.s <= 0.0) {       // < is bogus, just shuts up warnings
        r = in.v;
        g = in.v;
        b = in.v;
        return GradientColor(r,g,b);
    }
    hh = in.h;
    if(hh >= 360.0) hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = in.v * (1.0 - in.s);
    q = in.v * (1.0 - (in.s * ff));
    t = in.v * (1.0 - (in.s * (1.0 - ff)));

    switch(i) {
    case 0:
        r = in.v;
        g = t;
        b = p;
        break;
    case 1:
        r = q;
        g = in.v;
        b = p;
        break;
    case 2:
        r = p;
        g = in.v;
        b = t;
        break;

    case 3:
        r = p;
        g = q;
        b = in.v;
        break;
    case 4:
        r = t;
        g = p;
        b = in.v;
        break;
    case 5:
    default:
        r = in.v;
        g = p;
        b = q;
        break;
    }
    return GradientColor(r,g,b);     
}

GradientColor GradientColor::fromHsv(double h, double s, double v) {
    return fromHsv(HsvCoordinates(h, s, v));
}

GradientColor::CmykCoordinates GradientColor::toCmyk() {
    double k = 1-std::max(std::max(r, g), b);
    if (fabs(k-1) < 0.00001) {
        return CmykCoordinates(0, 0, 0, 100);
    }
    double c = (1-r-k)/(1-k);
    double m = (1-g-k)/(1-k);
    double y = (1-b-k)/(1-k);
    return CmykCoordinates(c*100, m*100, y*100, k*100);
}

GradientColor GradientColor::fromCmyk(GradientColor::CmykCoordinates cmyk) {
    GradientColor color;
    cmyk.c /= 100;
    cmyk.m /= 100;
    cmyk.y /= 100;
    cmyk.k /= 100;
    color.r = (1-cmyk.c)*(1-cmyk.k);
    color.g = (1-cmyk.m)*(1-cmyk.k);
    color.b = (1-cmyk.y)*(1-cmyk.k);
    return color;
}

GradientColor GradientColor::fromCmyk(double c, double m, double y, double k) {
    return fromCmyk(CmykCoordinates(c,m,y,k));
}

/* Converting to/from LAB colorspace accurately requires information about the device
 * display. We use an approximation from https://web.archive.org/web/20111111073514/http://www.easyrgb.com/index.php?X=MATH&H=08#text8
 */
GradientColor::LabCoordinates GradientColor::toLab() {
    LabCoordinates lab;

    double nr = rgbXyzFunction(r);
    double ng = rgbXyzFunction(g);
    double nb = rgbXyzFunction(b);

    double x = nr * 0.4142 + ng * 0.3576 + nb * 0.1805;
    double y = nr * 0.2126 + ng * 0.7152 + nb * 0.0722;
    double z = nr * 0.0193 + ng * 0.1192 + nb * 0.9505;

    double nx = xyzLabFunction(x / 0.95047);
    double ny = xyzLabFunction(y / 1.00000);
    double nz = xyzLabFunction(z / 1.08883);

    lab.l = 116 * ny - 16;
    lab.a = 500 * (nx - ny);
    lab.b = 200 * (ny - nz);

    return lab;
}

double GradientColor::rgbXyzFunction(double v) {
    if (v > 0.04045) {
        return pow((v + 0.055)/1.055, 2.4);
    } else {
        return v / 12.92;
    }
}

double GradientColor::xyzLabFunction(double v) {
    if (v > 0.008856) {
        return pow(7.787*v, 1.0/3.0);
    } else {
        return 7.787 * v + (16.0 / 116.0);
    }
}

GradientColor GradientColor::fromLab(GradientColor::LabCoordinates lab) {
    GradientColor color;

    double y = (lab.l + 16.0) / 116.0;
    double x = (lab.a / 500.0) + y;
    double z = y - (lab.b / 200.0);

    double nx = 0.95047 * labXyzFunction(x);
    double ny = 1.00000 * labXyzFunction(y);
    double nz = 1.08883 * labXyzFunction(z);

    double r =  3.2406 * nx - 1.5372 * ny - 0.4986 * nz;
    double g = -0.9689 * nx + 1.8758 * ny + 0.0415 * nz;
    double b =  0.0557 * nx - 0.2040 * ny + 1.0570 * nz;

    color.r = xyzRgbFunction(r);
    color.g = xyzRgbFunction(g);
    color.b = xyzRgbFunction(b);

    return color;
}

GradientColor GradientColor::fromLab(double l, double a, double b) {
    return fromLab(LabCoordinates(l, a, b));
}

double GradientColor::labXyzFunction(double v) {
    if (v > 0.206893) {
        return pow(v, 3.0);
    } else {
        return (v - 16.0 / 116.0) / 7.787;
    }
}

double GradientColor::xyzRgbFunction(double v) {
    if (v > 0.0031308) {
        return 1.055 * pow(v, 1.0 / 2.4) - 0.055;
    } else {
        return 12.92 * v;
    }
}

GradientColor GradientColor::fromGrayscale(double gs) {
    return GradientColor(gs, gs, gs);
}

GradientStop::GradientStop(double location_): location(location_) { }

OpacityStop::OpacityStop(double location, double opacity_): GradientStop(location), opacity(opacity_) { }

ColorStop::ColorStop(double location, GradientColor color_): GradientStop(location), color(color_) { }

Gradient::Gradient() { }

Gradient::Gradient(const Grd5SolidGradient& grd5Gradient): title(grd5Gradient.title.toString()) {
    colorStops.reserve(grd5Gradient.colorStops.size());
    auto prevStop = grd5Gradient.colorStops.at(0);
    colorStops.emplace_back(prevStop.Lctn / 4096.0, getGradientColor(prevStop.color));
    for (int i=1; i<grd5Gradient.colorStops.size(); i++) {
        auto colorStop = grd5Gradient.colorStops.at(i);
        auto midpoint = colorStop.Mdpn;
        auto color = getGradientColor(colorStop.color);
        if (midpoint != 50) {
            auto prevColor = getGradientColor(prevStop.color);
            double offset = (midpoint*prevStop.Lctn + (100-midpoint)*colorStop.Lctn) / 100.0;
            auto midpointColor = GradientColor(0.5*(prevColor.r+color.r),0.5*(prevColor.g+color.g),0.5*(prevColor.b+color.b));
            colorStops.emplace_back(offset / 4096.0, midpointColor);
        }
        colorStops.emplace_back(colorStop.Lctn / 4096.0, color);
        prevStop = colorStop;
    }
    opacityStops.reserve(grd5Gradient.opacityStops.size());
    for (auto opacityStop: grd5Gradient.opacityStops) {
        opacityStops.emplace_back(opacityStop.Lctn / 4096.0, opacityStop.Opct);
    }
}

GradientColor Gradient::getGradientColor(std::shared_ptr<grad::Grd5Color> color) {
    if (auto c = std::dynamic_pointer_cast<grad::Grd5RgbColor>(color)) {
        return GradientColor(c->Rd / 255.0, c->Grn / 255.0, c->Bl / 255.0);
    } else if (auto c = std::dynamic_pointer_cast<grad::Grd5HsvColor>(color)) {
        return GradientColor::fromHsv(c->H, c->Strt, c->Brgh);
    } else if (auto c = std::dynamic_pointer_cast<grad::Grd5CmykColor>(color)) {
        return GradientColor::fromCmyk(c->Cyn, c->Mgnt, c->Ylw, c->Blck);
    } else if (auto c = std::dynamic_pointer_cast<grad::Grd5LabColor>(color)) {
        return GradientColor::fromLab(c->Lmnc, c->A, c->B);
    } else if (auto c = std::dynamic_pointer_cast<grad::Grd5GrayScaleColor>(color)) {
        return GradientColor::fromGrayscale(c->val / 255.0);
    } else if (auto c = std::dynamic_pointer_cast<grad::Grd5ForegroundColor>(color)) {
        return GradientColor::fromGrayscale(1.0);
    } else if (auto c = std::dynamic_pointer_cast<grad::Grd5BackgroundColor>(color)) {
        return GradientColor::fromGrayscale(0.0);
    }
    throw std::invalid_argument("unknown GRD5 color");
}

void Gradient::acceptSerializer(Serializer& serializer) {
    // TODO
}

void Gradient::acceptDeserializer(Deserializer& deserializer) {
    // TODO
}

void Gradient::exportToSvg(SvgDocument& svgDoc) {
    auto linearGrad = svgDoc.newLinearGradientElement();
    // HACK: tinyxml2 writes the attirbutes in the order they were set,
    // meaning that if we write the id first, it will be written as the
    // leftmost attribute in the final XML file. Although the order
    // does not matter for functionalities, having ID's at the beginning
    // is more readable. This id attribute will be overwritten by
    // SvgDocument when it writes the XML, but setting it now
    // avoids having the id attribute all the way to the right.
    linearGrad->SetAttribute("id", "");

    linearGrad->SetAttribute("data-name", title.c_str());
    linearGrad->SetAttribute("gradientUnits", "objectBoundingBox");
    linearGrad->SetAttribute("spreadMethod", "pad");
    linearGrad->SetAttribute("x1", "0%");
    linearGrad->SetAttribute("x2", "100%");
    linearGrad->SetAttribute("y1", "0%");
    linearGrad->SetAttribute("y2", "0%");

    struct compareStops {
        bool operator()(GradientStop* a, GradientStop* b) const {
            return (a->location == b->location) ? a < b : (a->location < b->location);
        }
    };
    std::set<GradientStop*, compareStops> stops;
    for (int i=0; i<colorStops.size(); i++) {
        stops.insert(&colorStops[i]);
    }
    for (int i=0; i<opacityStops.size(); i++) {
        stops.insert(&opacityStops[i]);
    }

    auto it = stops.begin();
    auto prevColor = colorStops[0].color;
    auto prevOp = opacityStops[0].opacity;

    if (fabs((*it)->location) > EPS) {
        auto stop = getStopNode(0.0, prevColor, prevOp, svgDoc);
        linearGrad->InsertEndChild(stop);
    }

    for (auto colorStop: colorStops) {
        auto stop = getStopNode(colorStop.location, colorStop.color, 1.0, svgDoc);
        linearGrad->InsertEndChild(stop);
    }
    svgDoc.addLinearGradient(linearGrad);
}

tinyxml2::XMLNode* Gradient::getStopNode(double location, GradientColor color, double opacity, SvgDocument& svgDoc) {
    auto stop = svgDoc.newStopElement();

    stop->SetAttribute("offset", SvgDocument::percentageString(location).c_str());

    uint8_t r = lround(std::clamp(color.r, 0.0, 1.0) * 255.0);
    uint8_t g = lround(std::clamp(color.g, 0.0, 1.0) * 255.0);
    uint8_t b = lround(std::clamp(color.b, 0.0, 1.0) * 255.0);

    std::string colorString = "rgb("+to_string(r)+ ", " + std::to_string(g) + ", "
        + std::to_string(b) + ")";

    stop->SetAttribute("stop-color", colorString.c_str());
    stop->SetAttribute("stop-opacity", SvgDocument::formattedDouble(1.0).c_str());

    return stop;
}

bool Gradient::hasMultipleStops(double x) {
    auto cmpColorStops = [](ColorStop a, ColorStop b) { return a.location < b.location; };
    auto cmpOpacityStops = [](OpacityStop a, OpacityStop b) { return a.location < b.location; };
    auto testColorStop = ColorStop(x, GradientColor());
    auto testOpacityStop = OpacityStop(x, 0);

    int colorIdxLower = std::distance(colorStops.begin(), std::lower_bound(colorStops.begin(), colorStops.end(), testColorStop, cmpColorStops));
    int colorIdxUpper = std::distance(colorStops.begin(), std::upper_bound(colorStops.begin(), colorStops.end(), testColorStop, cmpColorStops));

    int opctyIdxLower = std::distance(opacityStops.begin(), std::lower_bound(opacityStops.begin(), opacityStops.end(), testOpacityStop, cmpOpacityStops));
    int opctyIdxUpper = std::distance(opacityStops.begin(), std::upper_bound(opacityStops.begin(), opacityStops.end(), testOpacityStop, cmpOpacityStops));

    return (colorIdxUpper - colorIdxLower > 1) || (opctyIdxUpper - opctyIdxLower > 1);
}

GradientColor Gradient::colorAtLeft(double x) {
    size_t n = colorStops.size();
    auto cmpStops = [](ColorStop a, ColorStop b) { return a.location < b.location; };
    auto it = std::distance(colorStops.begin(),
        std::lower_bound(colorStops.begin(), colorStops.end(), ColorStop(x, GradientColor()), cmpStops));

    if (it == 0) {
        return colorStops[0].color;
    }
    if (it == n) {
        return colorStops[n-1].color;
    }

    return colorAtSegment(x, it-1);
}

GradientColor Gradient::colorAtRight(double x) {
    size_t n = colorStops.size();
    auto cmpStops = [](ColorStop a, ColorStop b) { return a.location < b.location; };
    auto it = std::distance(colorStops.begin(),
        std::upper_bound(colorStops.begin(), colorStops.end(), ColorStop(x, GradientColor()), cmpStops));

    if (it == 0) {
        return colorStops[0].color;
    }
    if (it == n) {
        return colorStops[n-1].color;
    }

    return colorAtSegment(x, it-1);
}

GradientColor Gradient::colorAtSegment(double x, int begin) {
    double loc1 = colorStops[begin].location;
    double loc2 = colorStops[begin+1].location;
    if (fabs(loc2-loc1) < EPS) {
        return colorStops[begin].color;
    }
    auto c1 = colorStops[begin].color;
    auto c2 = colorStops[begin+1].color;
    double xt = colorStops[begin+1].location - colorStops[begin].location;
    double x1 = (x- colorStops[begin].location)/xt;
    double x2 = (colorStops[begin+1].location-x)/xt;
    return GradientColor(x1*c1.r+x2*c2.r, x1*c1.g+x2*c2.g, x1*c1.b+x2*c2.b);
}

SvgDocument::SvgDocument() {
    svgRoot = xmlDoc.NewElement("svg");
    svgRoot->SetAttribute("version", "1.1");
    svgRoot->SetAttribute("xmlns", "http://www.w3.org/2000/svg");
    xmlDoc.InsertEndChild(svgRoot);
}

std::string SvgDocument::percentageString(double p) {
    std::stringstream buffer;
    buffer<<std::setprecision(4)<<std::fixed<<(100.0*p)<<"%";
    auto ans = buffer.str();
    if (ans == "-0.0000%") {
        ans = "0.0000%";
    }
    return ans;
}

std::string SvgDocument::formattedDouble(double d) {
    std::stringstream buffer;
    buffer<<std::setprecision(4)<<std::fixed<<d;
    auto ans = buffer.str();
    if (ans == "-0.0000") {
        ans = "0.0000";
    }
    return ans;
}

void SvgDocument::addLinearGradient(XMLNode* node) {
    const char* buf = NULL;
    auto element = node->ToElement();
    if (element == NULL) {
        throw std::invalid_argument("Invalid gradient node");
    }
    auto err = element->QueryStringAttribute("data-name", &buf);
    if (err != tinyxml2::XML_SUCCESS) {
        throw std::invalid_argument("Invalid gradient node");
    }
    gradientMap.insert({idForName(buf), element});
    gradientVector.push_back(element);
}

XMLElement* SvgDocument::newLinearGradientElement() {
    return xmlDoc.NewElement("linearGradient");
}

XMLElement* SvgDocument::newStopElement() {
    return xmlDoc.NewElement("stop");
}

void SvgDocument::flushAndWriteToFile(std::string filename) {
    svgRoot->DeleteChildren();
    populateRoot();
    tinyxml2::XMLPrinter xmlPrinter;
    xmlDoc.Print(&xmlPrinter);
    std::ofstream fs(filename);
    fs << xmlPrinter.CStr();
    fs.clear();
    gradientMap.clear();
    gradientVector.clear();
}

std::string SvgDocument::idForName(std::string name) {
    std::regex invalidChars("[^a-zA-Z0-9\\.-_]+");
    return std::string("id_") + std::regex_replace(name, invalidChars, "_");
}

void SvgDocument::populateRoot() {
    if (gradientMap.empty()) {
        return;
    }
    auto it1 = gradientMap.begin();
    it1->second->SetAttribute("id", it1->first.c_str());
    auto it2 = std::next(it1);
    int idx = 0;
    while (it2 != gradientMap.end()) {
        if (it2->first == it1->first) {
            idx++;
        } else {
            idx = 0;
        }
        auto idStr = it2->first + (idx > 0 ? (std::string("_") + std::to_string(idx)) : "");
        it2->second->SetAttribute("id", idStr.c_str());
        it1++;
        it2++;
    }
    for (auto g: gradientVector) {
        svgRoot->InsertEndChild(g);
    }
}

}