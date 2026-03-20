#pragma once

#include <tinyxml2.h>
#include <set>
#include <string>

namespace core {

class FlameV;
class BaseXFormV;
class XFormV;
class FinalXFormV;
class PaletteV;

class SerializableV;

class Serializer {
public:
    virtual void serialize(FlameV& flame) = 0;
    virtual void serialize(XFormV& xform) = 0;
    virtual void serialize(FinalXFormV& finalXform) = 0;
    virtual void serialize(PaletteV& palette) = 0;
};

class Deserializer {
public:
    virtual void deserialize(FlameV& flame) = 0;
    virtual void deserialize(XFormV& xform) = 0;
    virtual void deserialize(FinalXFormV& finalXform) = 0;
    virtual void deserialize(PaletteV& palette) = 0;
};

class SerializableV {
public:
    virtual void acceptSerializer(Serializer& serializer) = 0;
    virtual void acceptDeserializer(Deserializer& deserializer) = 0;
};

class XmlSerializer: public Serializer {
public:
    XmlSerializer(tinyxml2::XMLDocument* xmlDoc, tinyxml2::XMLNode* parent);
    void serialize(FlameV& flame) override;
    void serialize(XFormV& xform) override;
    void serialize(FinalXFormV& finalXform) override;
    void serialize(PaletteV& palette) override;
private:
    void serializeBaseXForm(BaseXFormV& xform, tinyxml2::XMLElement* element);
    tinyxml2::XMLDocument* xmlDoc;
    tinyxml2::XMLNode* parent;
};

class XmlDeserializer: public Deserializer {
public:
    XmlDeserializer(tinyxml2::XMLNode* parent);
    void deserialize(FlameV& flame) override;
    void deserialize(XFormV& xform) override;
    void deserialize(FinalXFormV& finalXform) override;
    void deserialize(PaletteV& palette) override;
private:
    void deserializeBaseXForm(BaseXFormV& xform, tinyxml2::XMLElement* element);
    tinyxml2::XMLNode* parent;
    std::set<std::string> variationAttributeNames;
};

}