#pragma once

#include "variation.hpp"

namespace render {

struct XFormCL {
    static const int MAX_VARIATIONS = 10;
    struct VariationData {
        Variation::VariationID id;
        float weight;
    };
    VariationData varData[MAX_VARIATIONS];
    float a, b, c, d, e, f;
    float pa, pb, pc, pd, pe, pf;
};

}