#pragma once

#include <list>
#include <map>
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
    void serialize(FILE* fp);
    void deserialize(FILE* fp);
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
        auto it = std::next(list.begin(), index);
        return std::static_pointer_cast<T>(*it);
    }

    void append(std::shared_ptr<T> element) {
        list.push_back(element);
    }

    void remove(int index) {
        int count = list.size();
        if (index < 0 || index > count-1) {
            throw std::invalid_argument("Attempted to remove out of list bounds");
        }
        auto it = std::next(list.begin(), index);
        list.erase(it);
    }

    void clear() {
        while (!empty()) {
            remove(0);
        }
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
    std::list<std::shared_ptr<XMLElementClass>> list;
};

}