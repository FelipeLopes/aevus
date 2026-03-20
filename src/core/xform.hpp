#pragma once

#include "../serial/xml_attribute.hpp"
#include "../serial/xml_element.hpp"
#include "custom_coefs.hpp"
#include "serializable.hpp"
#include "variation.hpp"

namespace core {

struct XFormCL {
    float a, b, c, d, e, f;
    float pa, pb, pc, pd, pe, pf;
    float color, colorSpeed;
    int varBegin, varEnd;
};

class BaseXForm: public serial::XMLElementClass {
public:
    BaseXForm(std::string tag);
    serial::XMLAttributeDouble color;
    serial::XMLMultiAttribute<VariationMap> variationMap;
    serial::XMLAttribute<CoefsAffine> coefs;
    serial::XMLAttribute<PostAffine> post;
    serial::XMLAttribute<Chaos> chaos;
    serial::XMLMultiAttribute<ColorSpeed> colorSpeed;
};

class XForm: public BaseXForm {
public:
    XForm();
    serial::XMLAttributeDouble weight;
};

class FinalXForm: public BaseXForm {
public:
    FinalXForm();
};

class BaseXFormV: public SerializableV {
public:
    BaseXFormV();
    virtual ~BaseXFormV();
    double color, colorSpeed;
    VariationMap variationMap;
    CoefsAffine coefs;
    PostAffine post;
    Chaos chaos;
};

class XFormV: public BaseXFormV {
public:
    XFormV();
    ~XFormV();
    double weight;
    virtual void acceptSerializer(Serializer& serializer);
    virtual void acceptDeserializer(Deserializer& deserializer);
};

class FinalXFormV: public BaseXFormV {
public:
    FinalXFormV();
    ~FinalXFormV();
    virtual void acceptSerializer(Serializer& serializer);
    virtual void acceptDeserializer(Deserializer& deserializer);
};

}