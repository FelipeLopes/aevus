#include "variation.hpp"
#include <boost/assign.hpp>

namespace render {

const std::map<Variation::VariationID, std::string> Variation::names =
    boost::assign::map_list_of
        (Variation::LINEAR, "linear")
        (Variation::SPHERICAL, "spherical");

}