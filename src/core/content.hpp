#pragma once

#include <array>
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

struct ColorContent {
    double color, colorSpeed;
    core::PaletteColors palette;
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

struct FlameContent {
    FrameContent frame;
    RenderContent render;
    std::vector<XFormContent> xforms;
    std::optional<XFormContent> finalXForm;
    core::PaletteColors palette;
};

enum XFormOperation {
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