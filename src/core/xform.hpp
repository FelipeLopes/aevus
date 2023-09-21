#pragma once

#include "../serial/xml_attribute.hpp"
#include "../serial/xml_element.hpp"
#include "custom_coefs.hpp"
#include "variation.hpp"

namespace core {

struct XFormCL {
    float a, b, c, d, e, f;
    float pa, pb, pc, pd, pe, pf;
    float color, colorSpeed;
    int varBegin, varEnd;
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
    XFormCL toXFormCL(int varBegin) const;
    void readVariationCLArray(std::vector<VariationCL>& vars);
};

}