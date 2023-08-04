#pragma once

#include "../serial/xml_serialization.hpp"
#include "custom_coefs.hpp"
#include "variation.hpp"

namespace core {

struct XFormCL {
    static const int MAX_VARIATIONS = 10;
    struct VariationData {
        Variation::VariationID id;
        float weight;
    };
    VariationData varData[MAX_VARIATIONS];
    float a, b, c, d, e, f;
    float pa, pb, pc, pd, pe, pf;
    float color, colorSpeed;
};

class XForm: public serial::XMLElementClass {
public:
    XForm();
    serial::XMLAttributeDouble weight;
    serial::XMLAttributeDouble color;
    serial::XMLMultiAttribute<VariationMap> variationMap;
    serial::XMLAttribute<CoefsAffine> coefs;
    serial::XMLAttribute<PostAffine> post;
    serial::XMLAttribute<Chaos> chaos;
    serial::XMLMultiAttribute<ColorSpeed> colorSpeed;
    XFormCL toXFormCL() const;
};

}