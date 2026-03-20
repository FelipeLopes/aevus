#include "xform.hpp"
#include "variation.hpp"

namespace core {

BaseXForm::BaseXForm() {
    variationMap.variations[Variation::VariationID::LINEAR] = VariationData(1.0, {});
    color = 0;
    colorSpeed = 0.5;
}

BaseXForm::~BaseXForm() { }

XForm::XForm() { 
    weight = 0.5;
}

XForm::~XForm() { }

FinalXForm::FinalXForm() { }

FinalXForm::~FinalXForm() { }

void XForm::acceptSerializer(Serializer& serializer) {
    serializer.serialize(*this);
}

void XForm::acceptDeserializer(Deserializer& deserializer) {
    deserializer.deserialize(*this);
}

void FinalXForm::acceptSerializer(Serializer& serializer) {
    serializer.serialize(*this);
}

void FinalXForm::acceptDeserializer(Deserializer& deserializer) {
    deserializer.deserialize(*this);
}

}