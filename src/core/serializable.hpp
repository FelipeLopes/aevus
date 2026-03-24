#pragma once

#include <tinyxml2.h>
#include <set>
#include <string>

namespace core {

std::string formattedDouble(double x);

class Flame;
class BaseXForm;
class XForm;
class FinalXForm;
class Palette;

class Serializable;

class Serializer {
public:
    virtual void serialize(Flame& flame) = 0;
    virtual void serialize(XForm& xform) = 0;
    virtual void serialize(FinalXForm& finalXform) = 0;
    virtual void serialize(Palette& palette) = 0;
};

class Deserializer {
public:
    virtual void deserialize(Flame& flame) = 0;
    virtual void deserialize(XForm& xform) = 0;
    virtual void deserialize(FinalXForm& finalXform) = 0;
    virtual void deserialize(Palette& palette) = 0;
};

class Serializable {
public:
    virtual void acceptSerializer(Serializer& serializer) = 0;
    virtual void acceptDeserializer(Deserializer& deserializer) = 0;
};

class XmlSerializer: public Serializer {
public:
    XmlSerializer(tinyxml2::XMLDocument* xmlDoc, tinyxml2::XMLNode* parent);
    void serialize(Flame& flame) override;
    void serialize(XForm& xform) override;
    void serialize(FinalXForm& finalXform) override;
    void serialize(Palette& palette) override;
private:
    void serializeBaseXForm(BaseXForm& xform, tinyxml2::XMLElement* element);
    tinyxml2::XMLDocument* xmlDoc;
    tinyxml2::XMLNode* parent;
};

class XmlDeserializer: public Deserializer {
public:
    XmlDeserializer(tinyxml2::XMLNode* parent);
    void deserialize(Flame& flame) override;
    void deserialize(XForm& xform) override;
    void deserialize(FinalXForm& finalXform) override;
    void deserialize(Palette& palette) override;
private:
    void deserializeBaseXForm(BaseXForm& xform, tinyxml2::XMLElement* element);
    tinyxml2::XMLNode* parent;
    std::set<std::string> variationAttributeNames;
};

}