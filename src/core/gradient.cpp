#include "gradient.hpp"
#include "serializable.hpp"
#include <cmath>
#include <memory>

using grad::Grd5SolidGradient;

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
GradientColor fromHsv(GradientColor::HsvCoordinates in) {
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
        }
    }
}

void Gradient::acceptSerializer(Serializer& serializer) {
    // TODO
}

void Gradient::acceptDeserializer(Deserializer& deserializer) {
    // TODO
}


}