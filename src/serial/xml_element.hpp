#pragma once

#include <memory>
#include <string>
#include <vector>
#include "xml_attribute.hpp"
#include "xml_content.hpp"

namespace serial {

class XMLElementClass {
public:
    XMLElementClass(std::string tag);
    XMLElementClass(XMLElementClass& parent, std::string tag);
    void serialize(std::ostream& stream);
    void deserialize(std::istream& stream);
    std::string tag;
    std::vector<XMLElementClass*> children;
    std::vector<XMLAttributeField*> attributeFields;
    XMLContentClass* content;
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
        static_assert(std::is_default_constructible<T>::value,
            "T must have a default constructor");
    }

    std::shared_ptr<T> get(int index) const {
        int count = list.size();
        if (index < 0 || index > count-1) {
            throw std::invalid_argument("Attempted to access out of list bounds");
        }
        return std::static_pointer_cast<T>(list[index]);
    }

    void set(int index, std::shared_ptr<T> val) {
        int count = list.size();
        if (index < 0 || index > count-1) {
            throw std::invalid_argument("Attempted to access out of list bounds");
        }
        list[index] = val;
    }

    void appendAt(int pos, std::shared_ptr<T> element) {
        list.insert(list.begin()+pos, element);
    }

    void append(std::shared_ptr<T> element) {
        list.push_back(element);
    }

    void remove(int index) {
        int count = list.size();
        if (index < 0 || index > count-1) {
            throw std::invalid_argument("Attempted to remove out of list bounds");
        }
        list.erase(list.begin()+index);
    }

    void clear() {
        list.clear();
    }

    bool empty() const {
        return list.empty();
    }

    size_t size() const {
        return list.size();
    }

    virtual void nodeSerialize(tinyxml2::XMLDocument& xmlDoc, tinyxml2::XMLNode* parent) {
        for (auto el: list) {
            el->nodeSerialize(xmlDoc, parent);
        }
    }

    virtual tinyxml2::XMLNode* nodeDeserialize(tinyxml2::XMLNode* node) {
        clear();
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
    std::vector<std::shared_ptr<XMLElementClass>> list;
};

template <typename T>
class OptionalXMLElementClass: public XMLElementClass {
public:
    OptionalXMLElementClass(XMLElementClass& parent_, std::string tag_):
        XMLElementClass(parent_, tag_), parent(parent_), tag(tag_)
    {
        static_assert(std::is_base_of<XMLElementClass, T>::value,
            "T must inherit from XMLElementClass");
        static_assert(std::is_default_constructible<T>::value,
            "T must have a default constructor");
    }

    std::shared_ptr<T> get() const {
        return std::static_pointer_cast<T>(ptr);
    }

    void unset() {
        ptr.reset();
    }

    void set(std::shared_ptr<T> val) {
        ptr = val;
    }

    bool isSet() const {
        return ptr != nullptr;
    }

    virtual void nodeSerialize(tinyxml2::XMLDocument& xmlDoc, tinyxml2::XMLNode* parent) {
        if (isSet()) {
            ptr->nodeSerialize(xmlDoc, parent);
        }
    }

    virtual tinyxml2::XMLNode* nodeDeserialize(tinyxml2::XMLNode* node) {
        unset();
        if (node == NULL) {
            return node;
        }
        tinyxml2::XMLElement* element = node->ToElement();
        if (element == NULL) {
            return node;
        }
        std::string name = element->Name();
        if (name != tag) {
            return node;
        }
        auto clazz = std::make_shared<T>();
        clazz->nodeDeserialize(node);
        set(clazz);
        node = node->NextSibling();
        return node;
    }
private:
    XMLElementClass& parent;
    std::string tag;
    std::shared_ptr<XMLElementClass> ptr;
};

}