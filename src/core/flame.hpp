#pragma once

#include <cstdint>
#include <boost/bimap.hpp>
#include <optional>
#include "../serial/xml_element.hpp"
#include "color.hpp"
#include "palette.hpp"
#include "xform.hpp"

namespace core {

struct FlameCL {
    float cx, cy, scale;
    int width, height;
};

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

class Flame: public serial::XMLElementClass {
public:
    Flame();
    serial::XMLAttributeString version;
    serial::XMLAttributeString name;
    serial::XMLAttribute<SizeParams> size;
    serial::XMLAttribute<CenterParams> center;
    serial::XMLAttributeDouble scale;
    serial::XMLAttributeInt quality;
    serial::XMLAttribute<Color> background;
    serial::XMLAttributeDouble brightness;
    serial::XMLAttributeDouble contrast;
    serial::ListXMLElementClass<XForm> xforms;
    Palette palette;

    FlameCL getFlameCL() const;
    void readInitialStateArray(std::vector<IterationState>& arr, int size) const;
    void readXFormCLArray(std::vector<XFormCL>& arr) const;
    void readXFormDistribution(std::vector<uint8_t>& dist) const;

    const static int XFORM_DISTRIBUTION_GRAINS = 16384;
};

}