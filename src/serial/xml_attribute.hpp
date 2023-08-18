#pragma once

#include <boost/assign.hpp>
#include <functional>
#include <map>
#include <set>
#include <string>
#include <tinyxml2.h>

#include "serializable.hpp"

namespace serial {

class XMLElementClass;

class XMLAttributeField {
public:
    XMLAttributeField(XMLElementClass& parent, std::set<std::string> names);
    XMLAttributeField(XMLElementClass& parent, std::string name);
    XMLAttributeField(XMLElementClass& parent, std::function<void(std::set<std::string>&)> f);
    virtual std::map<std::string, std::string> serialize() = 0;
    virtual void deserialize(tinyxml2::XMLElement* element) = 0;
    virtual ~XMLAttributeField() { }
    std::set<std::string> names;
};

class XMLAttributeInt : public XMLAttributeField {
public:
    XMLAttributeInt(XMLElementClass& parent, std::string name);
    XMLAttributeInt(XMLElementClass& parent, std::string name, int defaultValue);
    virtual std::map<std::string, std::string> serialize();
    virtual void deserialize(tinyxml2::XMLElement* element);
    int getValue() const;
    void setValue(int value);
private:
    int val;
    const int defaultValue;
    const bool hasDefault;
};

class XMLAttributeDouble : public XMLAttributeField {
public:
    XMLAttributeDouble(XMLElementClass& parent, std::string name);
    XMLAttributeDouble(XMLElementClass& parent, std::string name, double defaultValue);
    virtual std::map<std::string, std::string> serialize();
    virtual void deserialize(tinyxml2::XMLElement* element);
    double getValue() const;
    void setValue(double value);
private:
    double val;
    const double defaultValue;
    const bool hasDefault;
};

class XMLAttributeString : public XMLAttributeField {
public:
    XMLAttributeString(XMLElementClass& parent, std::string name);
    XMLAttributeString(XMLElementClass& parent, std::string name, std::string defaultValue);
    virtual std::map<std::string, std::string> serialize();
    virtual void deserialize(tinyxml2::XMLElement* element);
    std::string getValue() const;
    void setValue(std::string value);
private:
    std::string val;
    const std::string defaultValue;
    const bool hasDefault;
};

template <typename T>
class XMLAttribute: public XMLAttributeField {
public:
    XMLAttribute(XMLElementClass& parent, std::string name): XMLAttributeField(parent, name) {
        static_assert(std::is_base_of<StringSerializable, T>::value,
            "T must implement StringSerializable interface");
        static_assert(std::is_default_constructible<T>::value,
            "T must have a default constructor");
    }
    virtual std::map<std::string, std::string> serialize() {
        std::string name = *names.begin();
        auto opt = val.toString();
        if (opt.has_value()) {
            return boost::assign::map_list_of(name, opt.value());
        } else {
            return std::map<std::string, std::string>();
        }
    }
    virtual void deserialize(tinyxml2::XMLElement* element) {
        const char* buf;
        std::optional<std::string> opt;
        std::string name = *names.begin();
        auto err = element->QueryStringAttribute(name.c_str(), &buf);
        if (err == tinyxml2::XML_NO_ATTRIBUTE) {
            opt = std::nullopt;
        } else if (err != tinyxml2::XML_SUCCESS) {
            auto ec = std::error_code(err, std::generic_category());
            std::string msg("Could not load string attribute with name ");
            throw std::system_error(ec, msg + name);
        } else {
            opt = buf;
        }
        val.fromString(opt);
    }
    T* get() {
        return &val;
    }
    T value() const {
        return val;
    }
private:
    T val;
};

template <typename T>
class XMLMultiAttribute: public XMLAttributeField {
public:
    XMLMultiAttribute(XMLElementClass& parent, std::set<std::string> names):
        XMLAttributeField(parent, names)
    {
        assertGeneric();
    }
    XMLMultiAttribute(XMLElementClass& parent, std::function<void(std::set<std::string>&)> f):
        XMLAttributeField(parent, f)
    {
        assertGeneric();
    }
    virtual std::map<std::string, std::string> serialize() {
        return val.toStringMap();
    }
    virtual void deserialize(tinyxml2::XMLElement* element) {
        const char* buf;
        std::map<std::string, std::string> stringMap;
        for (auto name: names) {
            auto err = element->QueryStringAttribute(name.c_str(), &buf);
            if (err == tinyxml2::XML_NO_ATTRIBUTE) {
                continue;
            }
            if (err != tinyxml2::XML_SUCCESS) {
                auto ec = std::error_code(err, std::generic_category());
                std::string msg("Could not load string attribute with name ");
                throw std::system_error(ec, msg + name);
            }
            stringMap[name] = buf;
        }
        val.fromStringMap(stringMap);
    }
    T getValue() const {
        return val;
    }
    void setValue(T value) {
        val = value;
    }
private:
    T val;
    void assertGeneric() {
        static_assert(std::is_base_of<StringMapSerializable, T>::value,
            "T must implement StringMapSerializable interface");
        static_assert(std::is_default_constructible<T>::value,
            "T must have a default constructor");
    }
};

}