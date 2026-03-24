#pragma once

#include "../grad/grd5_gradient.hpp"
#include "serializable.hpp"

namespace core {

// We use a separate color type for gradients, because
// floating point has more precision to rasterize it.
struct GradientColor {
    GradientColor();
    GradientColor(double r, double g, double b);
    double r, g, b;
    struct HsvCoordinates {
        double h, s, v;
    };
    HsvCoordinates toHsv();
    static GradientColor fromHsv(HsvCoordinates hsv);
};

struct OpacityStop {
    OpacityStop(double location, double opacity);
    double location, opacity;
};

struct ColorStop {
    ColorStop(double location, GradientColor color);
    double location;
    GradientColor color;
};

class Gradient: public Serializable {
public:
    Gradient();
    Gradient(const grad::Grd5SolidGradient& grd5Gradient);
    std::string title;
    std::vector<OpacityStop> opacityStops;
    std::vector<ColorStop> colorStops;
};

}