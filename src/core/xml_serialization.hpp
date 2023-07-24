#pragma once

#include <functional>
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
    XMLAttributeField(XMLElementClass& parent, std::set<std::string> names);
    XMLAttributeField(XMLElementClass& parent, std::string name);
    XMLAttributeField(XMLElementClass& parent, std::function<void(std::set<std::string>&)> f);
    virtual std::map<std::string, std::string> serialize() = 0;
    virtual void deserialize(tinyxml2::XMLElement* element) = 0;
    virtual ~XMLAttributeField() { }
//protected:
    std::set<std::string> names;
};

class XMLAttributeInt : public XMLAttributeField {
public:
    XMLAttributeInt(XMLElementClass& parent, std::string name);
    XMLAttributeInt(XMLElementClass& parent, std::string name, int defaultValue);
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
    XMLAttributeDouble(XMLElementClass& parent, std::string name);
    XMLAttributeDouble(XMLElementClass& parent, std::string name, double defaultValue);
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
    XMLAttributeString(XMLElementClass& parent, std::string name);
    XMLAttributeString(XMLElementClass& parent, std::string name, std::string defaultValue);
    virtual std::map<std::string, std::string> serialize();
    virtual void deserialize(tinyxml2::XMLElement* element);
    std::string getValue();
    void setValue(std::string value);
private:
    std::string val;
    const std::string defaultValue;
    const bool hasDefault;
};

class StringSerializable {
public:
    virtual std::string toString() = 0;
    virtual void fromString(std::string text) = 0;
    virtual ~StringSerializable() { }
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
        return boost::assign::map_list_of(name, val.toString());
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
        val.fromString(buf);
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

class StringMapSerializable {
public:
    virtual std::map<std::string, std::string> toStringMap() = 0;
    virtual void fromStringMap(std::map<std::string, std::string> stringMap) = 0;
    virtual ~StringMapSerializable() { }
};

template <typename T>
class XMLMultiAttribute: public XMLAttributeField {
public:
    XMLMultiAttribute(XMLElementClass& parent, std::function<void(std::set<std::string>&)> f):
        XMLAttributeField(parent, f)
    {
        static_assert(std::is_base_of<StringMapSerializable, T>::value,
            "T must implement StringMapSerializable interface");
        static_assert(std::is_default_constructible<T>::value,
            "T must have a default constructor");
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
    XMLContentString(XMLElementClass& parent);
    void deserialize(tinyxml2::XMLNode* node);
    std::string getValue();
    void setValue(std::string value);
private:
    std::string val;
};

class XMLElementClass {
public:
    XMLElementClass(std::string tag);
    XMLElementClass(XMLElementClass& parent, std::string tag);
    void serialize(FILE* fp);
    void deserialize(FILE* fp);
    std::string tag;
    std::vector<XMLElementClass*> children;
    std::map<std::string, std::list<std::shared_ptr<XMLElementClass>>> listTags;
    std::vector<XMLAttributeField*> attributeFields;
    XMLContentString* contentString;
    virtual ~XMLElementClass();
    virtual void nodeSerialize(tinyxml2::XMLDocument& xmlDoc, tinyxml2::XMLNode* parent);
    virtual tinyxml2::XMLNode* nodeDeserialize(tinyxml2::XMLNode* node);
};

template <typename T>
class ListXMLElementClass: public XMLElementClass {
public:
    ListXMLElementClass(XMLElementClass& parent_, std::string tag_):
        XMLElementClass(parent_, tag_), parent(parent_), tag(tag_)
    {
        static_assert(std::is_base_of<XMLElementClass, T>::value,
            "T must inherit from XMLElementClass");
        parent.listTags[tag] = std::list<std::shared_ptr<XMLElementClass>>();
        list = &parent.listTags[tag];
    }

    T get(int index) {
        int count = list->size();
        if (index < 0 || index > count-1) {
            throw std::invalid_argument("Attempted to access out of list bounds");
        }
        auto it = std::next(list->begin(), index);
        return *(*it);
    }

    void set(int index, std::shared_ptr<T> element) {
        int count = list->size();
        if (index < 0 || index > count-1) {
            throw std::invalid_argument("Attempted to access out of list bounds");
        }
        auto it = std::next(list->begin(), index);
        *it = element;
    }

    void append(std::shared_ptr<T> element) {
        list->push_back(element);
    }

    void remove(int index) {
        int count = list->size();
        if (index < 0 || index > count-1) {
            throw std::invalid_argument("Attempted to remove out of list bounds");
        }
        auto it = std::next(list->begin(), index);
        list->erase(it);
    }

    void clear() {
        while (!empty()) {
            remove(0);
        }
    }

    bool empty() {
        return list->empty();
    }

    ~ListXMLElementClass() {
        clear();
    }

    virtual void nodeSerialize(tinyxml2::XMLDocument& xmlDoc, tinyxml2::XMLNode* parent) {
        for (auto el: *list) {
            el->nodeSerialize(xmlDoc, parent);
        }
    }

    virtual tinyxml2::XMLNode* nodeDeserialize(tinyxml2::XMLNode* node) {
        while (true) {
            if (node == NULL) {
                break;
            }
            tinyxml2::XMLElement* element = node->ToElement();
            if (element == NULL) {
                break;
            }
            std::string name = element->Name();
            if (name != tag) {
                break;
            }
            auto clazz = std::make_shared<T>();
            clazz->nodeDeserialize(node);
            append(clazz);
            node = node->NextSibling();
        }
        return node;
    }
private:
    XMLElementClass& parent;
    std::string tag;
    std::list<std::shared_ptr<XMLElementClass>>* list;
};

}