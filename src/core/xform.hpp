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