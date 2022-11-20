#pragma once

#include <map>
#include <string>

namespace render {

struct Variation {
    enum VariationID {
        NO_VARIATION = -1,
        LINEAR = 0,
        SPHERICAL = 2
    };

    const static std::map<VariationID, std::string> names;
};

}