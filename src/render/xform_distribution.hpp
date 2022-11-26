#pragma once

#include <cinttypes>
#include <vector>

namespace render {

struct XFormDistribution {
    int numXForms;
    std::vector<uint8_t> data;
    const static int XFORM_DISTRIBUTION_GRAINS = 16384;
};

}