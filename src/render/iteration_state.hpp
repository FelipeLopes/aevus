#pragma once

#include <inttypes.h>

namespace render {

struct IterationState {
    float x, y, c;
    uint8_t xf;
    union SeedUnion {
        struct Seed {
            uint32_t low, hi;
        } word;
        uint64_t value;
    } seed;
};

}