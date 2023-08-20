#include "variation.hpp"
#include <boost/assign.hpp>
#include <stdexcept>

using boost::assign::list_of;
using boost::bimap;
using std::map;
using std::string;

namespace core {

const bimap<Variation::VariationID, string> Variation::variationNames =
    list_of<bimap<Variation::VariationID, string>::relation>
        (LINEAR, "linear")
        (SPHERICAL, "spherical")
        (POLAR, "polar")
        (HYPERBOLIC, "hyperbolic")
        (DIAMOND, "diamond")
        (EYEFISH, "eyefish")
        (CYLINDER, "cylinder")
        (SQUARE, "square");

VariationMap::VariationMap() { }

map<string, string> VariationMap::toStringMap() {
    map<string, string> ans;
    for (auto kv: variations) {
        auto it = Variation::variationNames.left.find(kv.first);
        if (it == Variation::variationNames.left.end()) {
            throw std::invalid_argument("Unknown variation ID");
        }
        ans[it->second] = serial::formattedDouble(kv.second);
    }
    return ans;
}

void VariationMap::fromStringMap(map<string, string> stringMap) {
    variations.clear();
    for (auto kv: stringMap) {
        auto it = Variation::variationNames.right.find(kv.first);
        if (it == Variation::variationNames.right.end()) {
            throw std::invalid_argument("Unknown variation name");
        }
        variations[it->second] = stod(kv.second);
    }
}

VariationLookup::VariationLookup() { }

string VariationLookup::idToName(Variation::VariationID id) const {
    auto it = Variation::variationNames.left.find(id);
    if (it == Variation::variationNames.left.end()) {
        throw std::invalid_argument("Invalid variation ID");
    }
    return it->second;
}

Variation::VariationID VariationLookup::nameToId(string name) const {
    auto it = Variation::variationNames.right.find(name);
    if (it == Variation::variationNames.right.end()) {
        throw std::invalid_argument("Invalid variation name");
    }
    return it->second;
}

}