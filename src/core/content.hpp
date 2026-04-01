#pragma once

#include <vector>
#include "custom_coefs.hpp"
#include "flame.hpp"
#include "palette.hpp"
#include "variation.hpp"

namespace core {

struct CoefsContent {
    double ox, oy, xx, xy, yx, yy;
};

struct XFormTriangleContent {
    int activeId;
    std::vector<CoefsContent> coefs;
};

struct WeightsContent {
    int activeId;
    std::vector<double> weights;
};

typedef std::map<core::Variation::VariationID, core::VariationData> VariationContent;

struct XFormColorContent {
    double color, colorSpeed;
};

struct FrameContent {
    core::SizeParams flameSize;
    core::CenterParams flameCenter;
    double flameScale;
};

struct RenderContent {
    double quality, brightness, contrast, gamma, vibrancy;
    Color background;
    ClippingMode clipping;
};

struct XFormContent {
    CoefsContent preCoefs, postCoefs;
    VariationContent variations;
    double weight, color, colorSpeed;
    Chaos chaos;
};

struct ColormapContent {
    double vectorWeight;
    core::PaletteColors palette;
    core::Gradient gradient;
    static const int PALETTE_WIDTH = 256;
    void computeColormapCL(std::vector<core::ColorCL>& arr) const {
        arr.resize(PALETTE_WIDTH);
        double rasterWeight = 1.0 - vectorWeight;
        for (int i=0; i<PALETTE_WIDTH; i++) {
            arr[i].r = rasterWeight * palette.paletteData[3*i] / 255.0;
            arr[i].g = rasterWeight * palette.paletteData[3*i+1] / 255.0;
            arr[i].b = rasterWeight * palette.paletteData[3*i+2] / 255.0;
            arr[i].a = 1.0;
        }
        int j = 0;
        int numStops = gradient.colorStops.size();
        if (numStops == 0) {
            return;
        }
        for (int i=0; i<PALETTE_WIDTH; i++) {
            double offset = (i + 0.) / PALETTE_WIDTH;
            while (j < numStops && offset > gradient.colorStops[j].location) {
                j++;
            }
            if (j == 0) {
                arr[i].r += vectorWeight * gradient.colorStops[0].color.r;
                arr[i].g += vectorWeight * gradient.colorStops[0].color.g;
                arr[i].b += vectorWeight * gradient.colorStops[0].color.b;
            } else if (j == numStops) {
                arr[i].r += vectorWeight * gradient.colorStops[numStops-1].color.r;
                arr[i].g += vectorWeight * gradient.colorStops[numStops-1].color.g;
                arr[i].b += vectorWeight * gradient.colorStops[numStops-1].color.b;
            } else {
                double xt = gradient.colorStops[j].location - gradient.colorStops[j-1].location;
                if (xt == 0) {
                    arr[i].r += vectorWeight * gradient.colorStops[j].color.r;
                    arr[i].g += vectorWeight * gradient.colorStops[j].color.g;
                    arr[i].b += vectorWeight * gradient.colorStops[j].color.b;
                } else {
                    double x1 = offset - gradient.colorStops[j-1].location;
                    double x2 = gradient.colorStops[j].location - offset;
                    double w1 = x2 / xt;
                    double w2 = x1 / xt;

                    arr[i].r += vectorWeight * (w1*gradient.colorStops[j-1].color.r+w2*gradient.colorStops[j].color.r);
                    arr[i].g += vectorWeight * (w1*gradient.colorStops[j-1].color.g+w2*gradient.colorStops[j].color.g);
                    arr[i].b += vectorWeight * (w1*gradient.colorStops[j-1].color.b+w2*gradient.colorStops[j].color.b);
                }
            }
        }
    }
};

struct FlameContent {
    FrameContent frame;
    RenderContent render;
    std::vector<XFormContent> xforms;
    std::optional<XFormContent> finalXForm;
    ColormapContent colormap;
};

enum XFormOperation {
    SELECTED,
    UPDATED,
    ADDED,
    REMOVED
};

struct ActiveXFormContent {
    int id;
    XFormOperation op;
    std::optional<XFormContent> xform;
};

struct ActiveXFormUpdateContent {
    std::optional<CoefsContent> preCoefs, postCoefs;
    std::optional<VariationContent> variations;
    std::optional<double> weight, color, colorSpeed;
};

}