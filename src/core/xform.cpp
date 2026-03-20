#include "xform.hpp"
#include "variation.hpp"

namespace core {

BaseXFormV::BaseXFormV() {
    variationMap.variations[Variation::VariationID::LINEAR] = VariationData(1.0, {});
    color = 0;
    colorSpeed = 0.5;
}

BaseXFormV::~BaseXFormV() { }

XFormV::XFormV() { 
    weight = 0.5;
}

XFormV::~XFormV() { }

FinalXFormV::FinalXFormV() { }

FinalXFormV::~FinalXFormV() { }

void XFormV::acceptSerializer(Serializer& serializer) {
    serializer.serialize(*this);
}

void XFormV::acceptDeserializer(Deserializer& deserializer) {
    deserializer.deserialize(*this);
}

void FinalXFormV::acceptSerializer(Serializer& serializer) {
    serializer.serialize(*this);
}

void FinalXFormV::acceptDeserializer(Deserializer& deserializer) {
    deserializer.deserialize(*this);
}

}