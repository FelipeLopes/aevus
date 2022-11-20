#pragma once

#include <map>
#include <string>

namespace render {

struct Variation {
    enum VariationID {
        NO_VAR = -1,
        LINEAR = 0,
        SPHERICAL = 2
    };

    const static std::map<VariationID, std::string> names;
};

}