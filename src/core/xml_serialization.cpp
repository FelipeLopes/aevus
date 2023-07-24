#include "xml_serialization.hpp"
#include <exception>
#include <iterator>
#include <stdexcept>
#include <string>

using std::function;
using std::map;
using std::set;
using std::string;
using std::to_string;
using tinyxml2::XMLDocument;
using tinyxml2::XMLElement;
using tinyxml2::XMLNode;
using boost::assign::list_of;
using boost::assign::map_list_of;

namespace core {

XMLAttributeField::XMLAttributeField(XMLElementClass& parent, set<string> names_):
    names(names_)
{
    parent.attributeFields.push_back(this);
}

XMLAttributeField::XMLAttributeField(XMLElementClass& parent, string name):
    names(list_of(name))
{
    parent.attributeFields.push_back(this);
}

XMLAttributeField::XMLAttributeField(XMLElementClass& parent, function<void(set<string>&)> f) {
    f(names);
    parent.attributeFields.push_back(this);
}

XMLAttributeInt::XMLAttributeInt(XMLElementClass& parent, string name):
    XMLAttributeField(parent, name),
    defaultValue(0),
    hasDefault(false) { }

XMLAttributeInt::XMLAttributeInt(XMLElementClass& parent, string name, int defaultValue_):
    XMLAttributeField(parent, name),
    defaultValue(defaultValue_),
    hasDefault(true) { }

map<string,string> XMLAttributeInt::serialize() {
    string name = *names.begin();
    return map_list_of(name, to_string(val));
}

void XMLAttributeInt::deserialize(XMLElement* element) {
    string name = *names.begin();
    if (hasDefault) {
        val = element->IntAttribute(name.c_str(), defaultValue);
    } else {
        auto err = element->QueryIntAttribute(name.c_str(), &val);
        if (err != tinyxml2::XML_SUCCESS) {
            auto ec = std::error_code(err, std::generic_category());
            string msg("Could not load int attribute with name ");
            throw std::system_error(ec, msg + name);
        }
    }
}

int XMLAttributeInt::getValue() {
    return val;
}

void XMLAttributeInt::setValue(int value) {
    val = value;
}

XMLAttributeDouble::XMLAttributeDouble(XMLElementClass& parent, string name):
    XMLAttributeField(parent, name),
    defaultValue(0.0),
    hasDefault(false) { }

XMLAttributeDouble::XMLAttributeDouble(XMLElementClass& parent, string name, double defaultValue_):
    XMLAttributeField(parent, name),
    defaultValue(defaultValue_),
    hasDefault(true) { }

map<string,string> XMLAttributeDouble::serialize() {
    string name = *names.begin();
    return map_list_of(name, to_string(val));
}

void XMLAttributeDouble::deserialize(XMLElement* element) {
    string name = *names.begin();
    if (hasDefault) {
        val = element->DoubleAttribute(name.c_str(), defaultValue);
    } else {
        auto err = element->QueryDoubleAttribute(name.c_str(), &val);
        if (err != tinyxml2::XML_SUCCESS) {
            auto ec = std::error_code(err, std::generic_category());
            string msg("Could not load double attribute with name ");
            throw std::system_error(ec, msg + name);
        }
    }
}

double XMLAttributeDouble::getValue() {
    return val;
}

void XMLAttributeDouble::setValue(double value) {
    val = value;
}

XMLAttributeString::XMLAttributeString(XMLElementClass& parent, string name):
    XMLAttributeField(parent, name),
    defaultValue(""),
    hasDefault(false) { }

XMLAttributeString::XMLAttributeString(XMLElementClass& parent, string name, string defaultValue_):
    XMLAttributeField(parent, name),
    defaultValue(defaultValue_),
    hasDefault(true) { }

map<string,string> XMLAttributeString::serialize() {
    string name = *names.begin();
    return map_list_of(name, val);
}

void XMLAttributeString::deserialize(XMLElement* element) {
    const char* buf;
    string name = *names.begin();
    if (hasDefault) {
        buf = element->Attribute(name.c_str(), defaultValue.c_str());
    } else {
        auto err = element->QueryStringAttribute(name.c_str(), &buf);
        if (err != tinyxml2::XML_SUCCESS) {
            auto ec = std::error_code(err, std::generic_category());
            string msg("Could not load string attribute with name ");
            throw std::system_error(ec, msg + name);
        }
    }
    val = buf;
}

string XMLAttributeString::getValue() {
    return val;
}

void XMLAttributeString::setValue(string value) {
    val = value;
}

XMLContentString::XMLContentString(XMLElementClass& parent) {
    parent.contentString = this;
}

void XMLContentString::deserialize(XMLNode* node) {
    val = node->Value();
}

string XMLContentString::getValue() {
    return val;
}

void XMLContentString::setValue(string value) {
    val = value;
}

XMLElementClass::XMLElementClass(string tag_): tag(tag_) {
    contentString = NULL;
}

XMLElementClass::XMLElementClass(XMLElementClass& parent, string tag_): tag(tag_) {
    contentString = NULL;
    parent.children.push_back(this);
}

void XMLElementClass::serialize(FILE* fp) {
    XMLDocument xmlDoc;
    nodeSerialize(xmlDoc, &xmlDoc);
    xmlDoc.SaveFile(stdout);
}

void XMLElementClass::deserialize(FILE* fp) {
    XMLDocument xmlDoc;
    auto err = xmlDoc.LoadFile(fp);
    if (err != tinyxml2::XML_SUCCESS) {
        auto ec = std::error_code(err, std::generic_category());
        throw std::system_error(ec, "Could not load XML file");
    }
    XMLNode* node = xmlDoc.FirstChild();
    nodeDeserialize(node);
    node = node->NextSibling();
    if (node != NULL) {
        throw std::invalid_argument("XMLDocument has more than one root");
    }
}

void XMLElementClass::nodeSerialize(XMLDocument& xmlDoc, XMLNode* parent) {
    XMLElement* element = xmlDoc.NewElement(tag.c_str());
    for (auto field: attributeFields) {
        auto attributeMap = field->serialize();
        for (auto kv: attributeMap) {
            element->SetAttribute(kv.first.c_str(), kv.second.c_str());
        }
    }
    for (auto child: children) {
        child->nodeSerialize(xmlDoc, element);
    }
    if (contentString != NULL) {
        element->SetText(contentString->getValue().c_str());
    }
    parent->InsertEndChild(element);
}

void XMLElementClass::nodeDeserialize(XMLNode* node) {
    if (node == NULL) {
        throw std::invalid_argument("XML document has no nodes");
    }
    XMLElement* element = node->ToElement();
    if (element == NULL) {
        throw std::invalid_argument("XML node is not an element");
    }
    string name = element->Name();
    if (name != tag) {
        throw std::invalid_argument("XML node has invalid tag");
    }
    for (auto field: attributeFields) {
        field->deserialize(element);
    }
    XMLNode* childNode = node->FirstChild();
    for (auto child: children) {
        child->nodeDeserialize(childNode);
        childNode = childNode->NextSibling();
    }
    if (contentString == NULL && childNode != NULL) {
        throw std::invalid_argument("XML node has incorrect number of children");
    }
    if (contentString != NULL) {
        if (childNode == NULL) {
            throw std::invalid_argument("XML node has no content");
        }
        contentString->deserialize(childNode);
    }
}

XMLElementClass::~XMLElementClass() { }

ListXMLElementClass::ListXMLElementClass(XMLElementClass& parent_, string tag_):
    XMLElementClass(parent_, tag_), parent(parent_), tag(tag_)
{
    parent.listTags[tag] = std::list<XMLElementClass*>();
    list = &parent.listTags[tag];
}

void ListXMLElementClass::append(XMLElementClass element) {
    XMLElementClass* listEl = new XMLElementClass(element);
    list->push_back(listEl);
}

void ListXMLElementClass::remove(int index) {
    int count = list->size();
    if (index < 0 || index > count-1) {
        throw std::invalid_argument("Attempted to remove out of list bounds");
    }
    auto it = std::next(list->begin(), index);
    delete *it;
    list->erase(it);
}

bool ListXMLElementClass::empty() {
    return list->empty();
}

ListXMLElementClass::~ListXMLElementClass() {
    while (!empty()) {
        remove(0);
    }
}

void ListXMLElementClass::nodeSerialize(XMLDocument& xmlDoc, XMLNode* parent) {
    for (auto el: *list) {
        el->nodeSerialize(xmlDoc, parent);
    }
}

void ListXMLElementClass::nodeDeserialize(XMLNode* node) {
    while (true) {
        if (node == NULL) {
            break;
        }
        XMLElement* element = node->ToElement();
        if (element == NULL) {
            break;
        }
        string name = element->Name();
        if (name != tag) {
            break;
        }
        XMLElementClass clazz(tag);
        clazz.nodeDeserialize(node);
        append(clazz);
        node = node->NextSibling();
    }
}

}