#pragma once

namespace core {

class FlameV;
class XFormV;
class FinalXFormV;
class PaletteV;
class ClippingV;

class SerializableV;

class Serializer {
public:
    virtual void serialize(FlameV& flame) = 0;
    virtual void serialize(XFormV& xform) = 0;
    virtual void serialize(FinalXFormV& finalXform) = 0;
    virtual void serialize(PaletteV& palette) = 0;
    virtual void serialize(ClippingV& clipping) = 0;
};

class Deserializer {
public:
    virtual void deserialize(FlameV& flame) = 0;
    virtual void deserialize(XFormV& xform) = 0;
    virtual void deserialize(FinalXFormV& finalXform) = 0;
    virtual void deserialize(PaletteV& palette) = 0;
    virtual void deserialize(ClippingV& clipping) = 0;
};

class SerializableV {
public:
    virtual void acceptSerializer(Serializer& serializer) = 0;
    virtual void acceptDeserializer(Deserializer& deserializer) = 0;
};

class XmlSerializer: public Serializer {
public:
    void serialize(FlameV& flame) override;
};

}