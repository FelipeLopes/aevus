#pragma once

#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>
#include <stdexcept>
#include <system_error>
#include <tinyxml2.h>
#include <boost/assign.hpp>

namespace core {

class XMLElementClass;

class XMLAttributeField {
public:
    XMLAttributeField(XMLElementClass& element, std::string name);
    virtual std::map<std::string, std::string> serialize() = 0;
    virtual void deserialize(tinyxml2::XMLElement* element) = 0;
    virtual ~XMLAttributeField() { }
protected:
    const std::set<std::string> names;
};

class XMLAttributeInt : public XMLAttributeField {
public:
    XMLAttributeInt(XMLElementClass& element, std::string name);
    XMLAttributeInt(XMLElementClass& element, std::string name, int defaultValue);
    virtual std::map<std::string, std::string> serialize();
    virtual void deserialize(tinyxml2::XMLElement* element);
    int getValue();
    void setValue(int value);
private:
    int val;
    const int defaultValue;
    const bool hasDefault;
};

class XMLAttributeDouble : public XMLAttributeField {
public:
    XMLAttributeDouble(XMLElementClass& element, std::string name);
    XMLAttributeDouble(XMLElementClass& element, std::string name, double defaultValue);
    virtual std::map<std::string, std::string> serialize();
    virtual void deserialize(tinyxml2::XMLElement* element);
    double getValue();
    void setValue(double value);
private:
    double val;
    const double defaultValue;
    const bool hasDefault;
};

class XMLAttributeString : public XMLAttributeField {
public:
    XMLAttributeString(XMLElementClass& element, std::string name);
    XMLAttributeString(XMLElementClass& element, std::string name, std::string defaultValue);
    virtual std::map<std::string, std::string> serialize();
    virtual void deserialize(tinyxml2::XMLElement* element);
    std::string getValue();
    void setValue(std::string value);
private:
    std::string val;
    const std::string defaultValue;
    const bool hasDefault;
};

class XMLSerializable {
public:
    virtual std::string toXMLString() = 0;
    virtual void fromXMLString(std::string text) = 0;
    virtual ~XMLSerializable() { }
};

template <typename T>
class XMLAttribute: public XMLAttributeField {
public:
    XMLAttribute(XMLElementClass& element, std::string name): XMLAttributeField(element, name) {
        static_assert(std::is_base_of<XMLSerializable, T>::value,
            "T must implement XMLSerializable interface");
        static_assert(std::is_default_constructible<T>::value,
            "T must have a default constructor");
    }
    virtual std::map<std::string, std::string> serialize() {
        std::string name = *names.begin();
        return boost::assign::map_list_of(name, val.toXMLString());
    }
    virtual void deserialize(tinyxml2::XMLElement* element) {
        const char* buf;
        std::string name = *names.begin();
        auto err = element->QueryStringAttribute(name.c_str(), &buf);
        if (err != tinyxml2::XML_SUCCESS) {
            auto ec = std::error_code(err, std::generic_category());
            std::string msg("Could not load string attribute with name ");
            throw std::system_error(ec, msg + name);
        }
        val.fromXMLString(buf);
    }
    T getValue() {
        return val;
    }
    void setValue(T value) {
        val = value;
    }
private:
    T val;
};

class XMLContentString {
public:
    XMLContentString(XMLElementClass& element);
    void deserialize(tinyxml2::XMLNode* node);
    std::string getValue();
    void setValue(std::string value);
private:
    std::string val;
};

class XMLElementClass {
public:
    XMLElementClass(std::string tag);
    XMLElementClass(XMLElementClass& element, std::string tag);
    void serialize(FILE* fp);
    void deserialize(FILE* fp);
    std::string tag;
    std::vector<XMLElementClass*> children;
    std::vector<XMLAttributeField*> attributeFields;
    XMLContentString* contentString;
private:
    tinyxml2::XMLNode* nodeSerialize(tinyxml2::XMLDocument& xmlDoc);
    void nodeDeserialize(tinyxml2::XMLNode* node);
};

}