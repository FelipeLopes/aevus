#pragma once

#include <string>
#include <tinyxml2.h>
#include "serializable.hpp"

namespace serial {

class XMLElementClass;

class XMLContentClass {
public:
    XMLContentClass(XMLElementClass& parent);
    virtual void deserialize(tinyxml2::XMLNode* node) = 0;
    virtual std::string serialize() = 0;
    virtual ~XMLContentClass() { }
};

class XMLContentString: public XMLContentClass {
public:
    XMLContentString(XMLElementClass& parent);
    virtual std::string serialize();
    virtual void deserialize(tinyxml2::XMLNode* node);
    void setValue(std::string value);
private:
    std::string val;
};

template <typename T>
class XMLContent: public XMLContentClass {
public:
    XMLContent(XMLElementClass& parent): XMLContentClass(parent) {
        static_assert(std::is_base_of<StringSerializable, T>::value,
            "T must implement StringSerializable interface");
        static_assert(std::is_default_constructible<T>::value,
            "T must have a default constructor");
    }

    virtual std::string serialize() {
        auto opt = val.toString();
        if (!opt.has_value()) {
            return "";
        } else {
            return opt.value();
        }
    }

    virtual void deserialize(tinyxml2::XMLNode* node) {
        val.fromString(node->Value());
    }

    T getValue() const {
        return val;
    }

    void setValue(T value) {
        val = value;
    }
private:
    T val;
};

}