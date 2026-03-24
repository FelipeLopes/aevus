#pragma once

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

class BaseXForm: public Serializable {
public:
    BaseXForm();
    virtual ~BaseXForm();
    double color, colorSpeed;
    VariationMap variationMap;
    CoefsAffine coefs;
    PostAffine post;
    Chaos chaos;
};

class XForm: public BaseXForm {
public:
    XForm();
    ~XForm();
    double weight;
    virtual void acceptSerializer(Serializer& serializer);
    virtual void acceptDeserializer(Deserializer& deserializer);
};

class FinalXForm: public BaseXForm {
public:
    FinalXForm();
    ~FinalXForm();
    virtual void acceptSerializer(Serializer& serializer);
    virtual void acceptDeserializer(Deserializer& deserializer);
};

}