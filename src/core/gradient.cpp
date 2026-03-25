#include "gradient.hpp"
#include "serializable.hpp"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <memory>
#include <regex>
#include <stdexcept>
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

OpacityStop::OpacityStop(double location_, double opacity_): location(location_), opacity(opacity_) { }

ColorStop::ColorStop(double location_, GradientColor color_): location(location_), color(color_) { }

Gradient::Gradient() { }

Gradient::Gradient(const Grd5SolidGradient& grd5Gradient): title(grd5Gradient.title.toString()) {
    colorStops.reserve(grd5Gradient.colorStops.size());
    for (auto colorStop: grd5Gradient.colorStops) {
        if (auto c = std::dynamic_pointer_cast<grad::Grd5RgbColor>(colorStop.color)) {
            colorStops.emplace_back(colorStop.Lctn, GradientColor(c->Rd, c->Grn, c->Bl));
        } else if (auto c = std::dynamic_pointer_cast<grad::Grd5HsvColor>(colorStop.color)) {
            colorStops.emplace_back(colorStop.Lctn, GradientColor::fromHsv(c->H, c->Strt, c->Brgh));
        } else if (auto c = std::dynamic_pointer_cast<grad::Grd5CmykColor>(colorStop.color)) {
            colorStops.emplace_back(colorStop.Lctn, GradientColor::fromCmyk(c->Cyn, c->Mgnt, c->Ylw, c->Blck));
        } else if (auto c = std::dynamic_pointer_cast<grad::Grd5LabColor>(colorStop.color)) {
            colorStops.emplace_back(colorStop.Lctn, GradientColor::fromLab(c->Lmnc, c->A, c->B));
        } else if (auto c = std::dynamic_pointer_cast<grad::Grd5GrayScaleColor>(colorStop.color)) {
            colorStops.emplace_back(colorStop.Lctn, GradientColor::fromGrayscale(c->val));
        }
    }
    opacityStops.reserve(grd5Gradient.opacityStops.size());
    for (auto opacityStop: grd5Gradient.opacityStops) {
        opacityStops.emplace_back(opacityStop.Lctn, opacityStop.Opct);
    }
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
    for (auto colorStop: colorStops) {
        auto stop = svgDoc.newStopElement();
        stop->SetAttribute("offset", colorStop.location);

        uint8_t r = lround(std::clamp(colorStop.color.r, 0.0, 1.0) * 255.0);
        uint8_t g = lround(std::clamp(colorStop.color.r, 0.0, 1.0) * 255.0);
        uint8_t b = lround(std::clamp(colorStop.color.r, 0.0, 1.0) * 255.0);

        std::string colorString = "rgb("+to_string(r)+ ", " + std::to_string(g) + ", "
            + std::to_string(b) + ")";

        stop->SetAttribute("stop-color", colorString.c_str());
        stop->SetAttribute("stop-opacity", 1.0);
        linearGrad->InsertEndChild(stop);
    }
    svgDoc.addLinearGradient(linearGrad);
}

SvgDocument::SvgDocument() {
    svgRoot = xmlDoc.NewElement("svg");
    svgRoot->SetAttribute("version", "1.1");
    svgRoot->SetAttribute("xmlns", "http://www.w3.org/2000/svg");
    xmlDoc.InsertEndChild(svgRoot);
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
    gradients.insert({idForName(buf), element});
}

XMLElement* SvgDocument::newLinearGradientElement() {
    return xmlDoc.NewElement("linearGradient");
}

XMLElement* SvgDocument::newStopElement() {
    return xmlDoc.NewElement("stop");
}

void SvgDocument::writeToFile(std::string filename) {
    svgRoot->DeleteChildren();
    populateRoot();
    tinyxml2::XMLPrinter xmlPrinter;
    xmlDoc.Print(&xmlPrinter);
    std::ofstream fs(filename);
    fs << xmlPrinter.CStr();
    fs.clear();
}

std::string SvgDocument::idForName(std::string name) {
    std::regex invalidChars("[^a-zA-Z0-9\\.-_]+");
    return std::string("id_") + std::regex_replace(name, invalidChars, "_");
}

void SvgDocument::populateRoot() {
    if (gradients.empty()) {
        return;
    }
    auto it1 = gradients.begin();
    it1->second->SetAttribute("id", it1->first.c_str());
    svgRoot->InsertEndChild(it1->second);
    auto it2 = std::next(it1);
    int idx = 0;
    while (it2 != gradients.end()) {
        if (it2->first == it1->first) {
            idx++;
        } else {
            idx = 0;
        }
        auto idStr = it2->first + (idx > 0 ? (std::string("_") + std::to_string(idx)) : "");
        it2->second->SetAttribute("id", idStr.c_str());
        svgRoot->InsertEndChild(it2->second);
        it1++;
        it2++;
    }
}

}