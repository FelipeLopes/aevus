#include "variation.hpp"
#include <boost/assign.hpp>
#include <boost/assign/list_of.hpp>
#include <stdexcept>

using boost::assign::list_of;
using boost::assign::map_list_of;
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
        (PDJ, "pdj")
        (EYEFISH, "eyefish")
        (CYLINDER, "cylinder")
        (SQUARE, "square");

const map<Variation::VariationID, VariationParamNames> Variation::variationParamNames =
    map_list_of
        (LINEAR, VariationParamNames())
        (SPHERICAL, VariationParamNames())
        (POLAR, VariationParamNames())
        (HYPERBOLIC, VariationParamNames())
        (DIAMOND, VariationParamNames())
        (PDJ, VariationParamNames({"a", "b", "c", "d"}))
        (EYEFISH, VariationParamNames())
        (CYLINDER, VariationParamNames())
        (SQUARE, VariationParamNames());

VariationMap::VariationMap() { }

map<string, string> VariationMap::toStringMap() {
    map<string, string> ans;
    for (auto kv: variations) {
        auto it = Variation::variationNames.left.find(kv.first);
        if (it == Variation::variationNames.left.end()) {
            throw std::invalid_argument("Unknown variation ID");
        }
        ans[it->second] = serial::formattedDouble(kv.second.weight);
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
        variations[it->second].weight = stod(kv.second);
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