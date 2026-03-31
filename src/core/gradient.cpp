#include "gradient.hpp"
#include "serializable.hpp"
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <memory>
#include <optional>
#include <regex>
#include <stdexcept>
#include <string>
#include <tinyxml2.h>
#include <lunasvg.h>

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

Gradient::Gradient() {
    opacityStops.emplace_back(0, 0);
    opacityStops.emplace_back(1, 0);

    colorStops.emplace_back(0, GradientColor(1.0, 1.0, 1.0));
    colorStops.emplace_back(1, GradientColor(1.0, 1.0, 1.0));
 }

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
            double offset = ((100-midpoint)*prevStop.Lctn + midpoint*colorStop.Lctn) / 100.0;
            auto midpointColor = GradientColor(0.5*(prevColor.r+color.r),0.5*(prevColor.g+color.g),0.5*(prevColor.b+color.b));
            colorStops.emplace_back(offset / 4096.0, midpointColor);
        }
        colorStops.emplace_back(colorStop.Lctn / 4096.0, color);
        prevStop = colorStop;
    }
    opacityStops.reserve(grd5Gradient.opacityStops.size());
    auto prevOp = grd5Gradient.opacityStops.at(0);
    opacityStops.emplace_back(prevOp.Lctn / 4096.0, prevOp.Opct / 100.0);
    for (int i=1; i<grd5Gradient.opacityStops.size(); i++) {
        auto opStop = grd5Gradient.opacityStops.at(i);
        auto midpoint = opStop.Mdpn;
        if (midpoint != 50) {
            double offset = ((100-midpoint)*prevOp.Lctn + midpoint*opStop.Lctn) / 100.0;
            double midpointOp = 0.5*(prevOp.Opct+opStop.Opct);
            opacityStops.emplace_back(offset / 4096.0, midpointOp / 100.0);
        }
        opacityStops.emplace_back(opStop.Lctn / 4096.0, opStop.Opct / 100.0);
        prevOp = opStop;
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

XMLElement* Gradient::getSvgElementWithoutId(SvgDocument& svgDoc) {
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

    std::set<double> stopLocations;
    for (int i=0; i<colorStops.size(); i++) {
        stopLocations.insert(colorStops[i].location);
    }
    for (int i=0; i<opacityStops.size(); i++) {
        stopLocations.insert(opacityStops[i].location);
    }

    for(auto it = stopLocations.begin(); it != stopLocations.end(); it++) {
        double x = (*it);
        auto color = colorAtLeft(x);
        double opacity = opacityAtLeft(x);
        auto stop = getStopNode(x, color, opacity, svgDoc);
        linearGrad->InsertEndChild(stop);
        if (hasMultipleStops(x)) {
            color = colorAtRight(x);
            opacity = opacityAtRight(x);
            stop = getStopNode(x, color, opacity, svgDoc);
            linearGrad->InsertEndChild(stop);
        }
    }

    return linearGrad;
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
    stop->SetAttribute("stop-opacity", SvgDocument::formattedDouble(opacity).c_str());

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
    return GradientColor(x2*c1.r+x1*c2.r, x2*c1.g+x1*c2.g, x2*c1.b+x1*c2.b);
}

double Gradient::opacityAtLeft(double x) {
    size_t n = opacityStops.size();
    auto cmpStops = [](OpacityStop a, OpacityStop b) { return a.location < b.location; };
    auto it = std::distance(opacityStops.begin(),
        std::lower_bound(opacityStops.begin(), opacityStops.end(), OpacityStop(x, 0.0), cmpStops));

    if (it == 0) {
        return opacityStops[0].opacity;
    }
    if (it == n) {
        return opacityStops[n-1].opacity;
    }

    return opacityAtSegment(x, it-1);
}

double Gradient::opacityAtRight(double x) {
    size_t n = opacityStops.size();
    auto cmpStops = [](OpacityStop a, OpacityStop b) { return a.location < b.location; };
    auto it = std::distance(opacityStops.begin(),
        std::upper_bound(opacityStops.begin(), opacityStops.end(), OpacityStop(x, 0.0), cmpStops));

    if (it == 0) {
        return opacityStops[0].opacity;
    }
    if (it == n) {
        return opacityStops[n-1].opacity;
    }

    return opacityAtSegment(x, it-1);
}

double Gradient::opacityAtSegment(double x, int begin) {
    double loc1 = opacityStops[begin].location;
    double loc2 = opacityStops[begin+1].location;
    if (fabs(loc2-loc1) < EPS) {
        return opacityStops[begin].opacity;
    }
    auto c1 = opacityStops[begin].opacity;
    auto c2 = opacityStops[begin+1].opacity;
    double xt = opacityStops[begin+1].location - opacityStops[begin].location;
    double x1 = (x-opacityStops[begin].location)/xt;
    double x2 = (opacityStops[begin+1].location-x)/xt;
    return c1*x2 + c2*x1;
}

void Gradient::generateDisplayImage(SvgDocument& svgDoc) {
    auto el = getSvgElementWithoutId(svgDoc);
    svgDoc.addLinearGradient(el);
    svgDoc.fillGradientIds();
    std::string id = svgDoc.getGradientId(0).value();
    svgDoc.addRect(id);
}

void Gradient::renderPNG(int width, int height, std::vector<uint8_t>& out) {
    std::stringstream buf;
    core::SvgDocument svgDoc;
    generateDisplayImage(svgDoc);
    svgDoc.writeToStream(buf);
    SvgDocument::renderStringToPNG(buf.str().c_str(), width, height, out);
}

SvgDocument::SvgDocument() {
    svgRoot = xmlDoc.NewElement("svg");
    svgRoot->SetAttribute("version", "1.1");
    svgRoot->SetAttribute("xmlns", "http://www.w3.org/2000/svg");
    xmlDoc.InsertEndChild(svgRoot);
    defsRoot = xmlDoc.NewElement("defs");
    svgRoot->InsertEndChild(defsRoot);
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

void SvgDocument::addRect(std::string fillId) {
    auto el = xmlDoc.NewElement("rect");
    el->SetAttribute("fill", (std::string("url('#")+fillId+"')").c_str());
    el->SetAttribute("width", "100%");
    el->SetAttribute("height", "100%");
    svgRoot->InsertEndChild(el);
}

void SvgDocument::setSize(int width, int height) {
    svgRoot->SetAttribute("width", width);
    svgRoot->SetAttribute("height", height);
}

XMLElement* SvgDocument::newLinearGradientElement() {
    return xmlDoc.NewElement("linearGradient");
}

XMLElement* SvgDocument::newStopElement() {
    return xmlDoc.NewElement("stop");
}

void SvgDocument::writeToStream(std::ostream& stream) {
    tinyxml2::XMLPrinter xmlPrinter;
    xmlDoc.Print(&xmlPrinter);
    stream << xmlPrinter.CStr();
    stream.clear();
}

void SvgDocument::clear() {
    defsRoot->DeleteChildren();
    gradientVector.clear();
    gradientMap.clear();
    auto rectNode = defsRoot->NextSibling();
    if (rectNode != NULL) {
        svgRoot->DeleteChild(rectNode);
    }
}

std::string SvgDocument::idForName(std::string name) {
    std::regex invalidChars("[^a-zA-Z0-9\\.-_]+");
    return std::string("id_") + std::regex_replace(name, invalidChars, "_");
}

void SvgDocument::fillGradientIds() {
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
        defsRoot->InsertEndChild(g);
    }
}

std::optional<std::string> SvgDocument::getGradientId(int idx) {
    if (idx < 0) {
        return std::nullopt;
    }
    int i = 0;
    XMLNode* node = defsRoot->FirstChild();
    while (i < idx && node != NULL) {
        i++;
        node = node->NextSibling();
    }
    if (node == NULL) {
        return std::nullopt;
    }
    XMLElement* element = node->ToElement();
    if (element == NULL) {
        throw std::invalid_argument("Invalid gradient node");
    }
    const char* buf = NULL;
    auto err = element->QueryStringAttribute("id", &buf);
    if (err != tinyxml2::XML_SUCCESS) {
        throw std::invalid_argument("Invalid gradient node");
    }
    return buf;
}

void SvgDocument::renderStringToPNG(const char* str, int width, int height, std::vector<uint8_t>& out) {
    auto bitmap = lunasvg::Document::loadFromData(str)->renderToBitmap(width, height);
    LunaSvgClosure closure(&out);
    bitmap.writeToPng(lunaSvgCallback, (void*)&closure);
}

void SvgDocument::lunaSvgCallback(void* closure, void* data, int size) {
    uint8_t* bytesData = (uint8_t*)data;
    static_cast<LunaSvgClosure*>(closure)->v->assign(bytesData, bytesData+size);
}

}