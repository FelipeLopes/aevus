#pragma once

#include "../grad/grd5_gradient.hpp"
#include "serializable.hpp"
#include <map>
#include <memory>
#include <optional>
#include <tinyxml2.h>

namespace core {

// We use a separate color type for gradients, because
// floating point has more precision to rasterize it.
struct GradientColor {
    GradientColor();
    GradientColor(double r, double g, double b);
    double r, g, b;
    struct HsvCoordinates {
        double h, s, v;
        HsvCoordinates();
        HsvCoordinates(double h_, double s_, double v_): h(h_), s(s_), v(v_) { }
    };
    HsvCoordinates toHsv();
    static GradientColor fromHsv(HsvCoordinates hsv);
    static GradientColor fromHsv(double h, double s, double v);
    struct CmykCoordinates {
        double c, m, y, k;
        CmykCoordinates();
        CmykCoordinates(double c_, double m_, double y_, double k_): c(c_), m(m_), y(y_), k(k_) { }
    };
    CmykCoordinates toCmyk();
    static GradientColor fromCmyk(CmykCoordinates cmyk);
    static GradientColor fromCmyk(double c, double m, double y, double k);
    struct LabCoordinates {
        double l, a, b;
        LabCoordinates();
        LabCoordinates(double l_, double a_, double b_): l(l_), a(a_), b(b_) { }
    };
    LabCoordinates toLab();
    static GradientColor fromLab(LabCoordinates lab);
    static GradientColor fromLab(double l, double a, double b);
    static GradientColor fromGrayscale(double gs);
private:
    static double rgbXyzFunction(double v);
    static double xyzLabFunction(double v);
    static double labXyzFunction(double v);
    static double xyzRgbFunction(double v);
};

struct GradientStop {
    GradientStop(double location);
    virtual ~GradientStop() = default;
    double location;
};

struct OpacityStop: public GradientStop {
    OpacityStop(double location, double opacity);
    double opacity;
};

struct ColorStop: public GradientStop {
    ColorStop(double location, GradientColor color);
    GradientColor color;
};

class SvgDocument {
public:
    SvgDocument();
    void addLinearGradient(tinyxml2::XMLNode* node);
    tinyxml2::XMLElement* newLinearGradientElement();
    tinyxml2::XMLElement* newStopElement();
    void fillGradientIds();
    void addRect(std::string fillId);
    void clear();
    std::optional<std::string> getGradientId(int idx);
    void writeToFile(std::string filename);
    static std::string percentageString(double p);
    static std::string formattedDouble(double d);
private:
    tinyxml2::XMLDocument xmlDoc;
    tinyxml2::XMLElement* svgRoot;
    tinyxml2::XMLElement* defsRoot;
    std::multimap<std::string, tinyxml2::XMLElement*> gradientMap;
    std::vector<tinyxml2::XMLElement*> gradientVector;
    std::string idForName(std::string name);
};

class Gradient: public Serializable {
public:
    Gradient();
    virtual ~Gradient() = default;
    Gradient(const grad::Grd5SolidGradient& grd5Gradient);
    std::string title;
    std::vector<OpacityStop> opacityStops;
    std::vector<ColorStop> colorStops;
    virtual void acceptSerializer(Serializer& serializer);
    virtual void acceptDeserializer(Deserializer& deserializer);
    tinyxml2::XMLElement* getSvgElementWithoutId(SvgDocument& svgDoc);
    void generateDisplayImage(SvgDocument& svgDoc);
private:
    GradientColor getGradientColor(std::shared_ptr<grad::Grd5Color> color);
    bool hasMultipleStops(double x);
    GradientColor colorAtLeft(double x);
    GradientColor colorAtRight(double x);
    GradientColor colorAtSegment(double x, int begin);
    double opacityAtLeft(double x);
    double opacityAtRight(double x);
    double opacityAtSegment(double x, int begin);
    tinyxml2::XMLNode* getStopNode(double location, GradientColor color, double opacity, SvgDocument& svgDoc);
    const double EPS = 1e-5;
};

}