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
    bool flameLoaded;
    std::vector<double> weights;
};

struct VariationContent {
    bool flameLoaded;
    std::map<core::Variation::VariationID, core::VariationData> variations;
};

struct ColorContent {
    bool flameLoaded;
    double color, colorSpeed;
    core::PaletteColors palette;
};

struct FrameContent {
    bool flameLoaded;
    core::SizeParams flameSize;
    core::CenterParams flameCenter;
    double flameScale;
};

}