#pragma once

#include <array>
#include <vector>
#include "../core/custom_coefs.hpp"
#include "../core/palette.hpp"
#include "../core/variation.hpp"

namespace ui {

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

struct XFormContent {
    CoefsContent preCoefs, postCoefs;
    VariationContent variations;
    double weight, color, colorSpeed;
};

struct FlameContent {
    FrameContent frame;
    std::vector<XFormContent> xforms;
    std::optional<XFormContent> finalXForm;
    core::PaletteColors palette;
};

struct ActiveXFormContent {
    int id;
    XFormContent xform;
};

struct ActiveXFormUpdateContent {
    std::optional<CoefsContent> preCoefs, postCoefs;
    std::optional<VariationContent> variations;
    std::optional<double> weight, color, colorSpeed;
};

struct AddedXFormContent {
    int id;
    XFormContent xform;
};

}