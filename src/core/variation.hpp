#pragma once

#include <boost/bimap.hpp>
#include "../serial/serializable.hpp"

namespace core {

struct Variation {
    enum VariationID {
        NO_VARIATION = -1,
        LINEAR = 0,
        SPHERICAL = 2,
        POLAR = 5,
        HYPERBOLIC = 10,
        DIAMOND = 11,
        EYEFISH = 27,
        CYLINDER = 29,
        SQUARE = 43
    };

    const static boost::bimap<VariationID, std::string> variationNames;
};

class VariationMap: public serial::StringMapSerializable {
public:
    VariationMap();
    virtual std::map<std::string, std::string> toStringMap();
    virtual void fromStringMap(std::map<std::string, std::string> stringMap);
    std::map<Variation::VariationID, double> variations;
};

class VariationLookup {
public:
    static const VariationLookup* const getInstance() {
        static const VariationLookup instance;
        return &instance;
    }
    std::string idToName(Variation::VariationID) const;
    Variation::VariationID nameToId(std::string name) const;
private:
    VariationLookup();
    VariationLookup(VariationLookup const&) = delete;
    void operator= (VariationLookup const&) = delete;
};

}