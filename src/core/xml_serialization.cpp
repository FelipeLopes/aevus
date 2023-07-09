#include "xml_serialization.hpp"
#include <exception>
#include <stdexcept>
#include <string>

using std::string;
using std::to_string;
using tinyxml2::XMLDocument;
using tinyxml2::XMLElement;
using tinyxml2::XMLNode;

namespace core {

XMLAttributeField::XMLAttributeField(XMLElementClass& element, string name) {
    element.attributes[name] = this;
}

XMLAttributeInt::XMLAttributeInt(XMLElementClass& element, string name):
    XMLAttributeField(element, name) { }

string XMLAttributeInt::serialize() {
    return to_string(val);
}

void XMLAttributeInt::deserialize(XMLElement* element, std::string name) {
    auto err = element->QueryIntAttribute(name.c_str(), &val);
    if (err != tinyxml2::XML_SUCCESS) {
        auto ec = std::error_code(err, std::generic_category());
        string msg("Could not load int attribute with name ");
        throw std::system_error(ec, msg + name);
    }
}

int XMLAttributeInt::getValue() {
    return val;
}

void XMLAttributeInt::setValue(int value) {
    val = value;
}

XMLAttributeDouble::XMLAttributeDouble(XMLElementClass& element, string name):
    XMLAttributeField(element, name) { }

string XMLAttributeDouble::serialize() {
    return to_string(val);
}

void XMLAttributeDouble::deserialize(XMLElement* element, std::string name) {
    auto err = element->QueryDoubleAttribute(name.c_str(), &val);
    if (err != tinyxml2::XML_SUCCESS) {
        auto ec = std::error_code(err, std::generic_category());
        string msg("Could not load double attribute with name ");
        throw std::system_error(ec, msg + name);
    }
}

double XMLAttributeDouble::getValue() {
    return val;
}

void XMLAttributeDouble::setValue(double value) {
    val = value;
}

XMLAttributeString::XMLAttributeString(XMLElementClass& element, string name):
    XMLAttributeField(element, name) { }

string XMLAttributeString::serialize() {
    return val;
}

void XMLAttributeString::deserialize(XMLElement* element, std::string name) {
    const char* buf;
    auto err = element->QueryStringAttribute(name.c_str(), &buf);
    if (err != tinyxml2::XML_SUCCESS) {
        auto ec = std::error_code(err, std::generic_category());
        string msg("Could not load string attribute with name ");
        throw std::system_error(ec, msg + name);
    }
    val = buf;
}

string XMLAttributeString::getValue() {
    return val;
}

void XMLAttributeString::setValue(string value) {
    val = value;
}

XMLContentString::XMLContentString(XMLElementClass& element) {
    element.contentString = this;
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

XMLElementClass::XMLElementClass(std::string tag_): tag(tag_) {
    contentString = NULL;
}

XMLElementClass::XMLElementClass(XMLElementClass& element, std::string tag_): tag(tag_) {
    element.children.push_back(this);
}

void XMLElementClass::serialize(FILE* fp) {
    XMLDocument xmlDoc;
    auto node = nodeSerialize(xmlDoc);
    xmlDoc.InsertFirstChild(node);
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

XMLNode* XMLElementClass::nodeSerialize(XMLDocument& xmlDoc) {
    XMLElement* element = xmlDoc.NewElement(tag.c_str());
    for (auto kv: attributes) {
        element->SetAttribute(kv.first.c_str(), kv.second->serialize().c_str());
    }
    for (auto child: children) {
        element->InsertEndChild(child->nodeSerialize(xmlDoc));
    }
    if (contentString != NULL) {
        element->SetText(contentString->getValue().c_str());
    }
    return element;
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
    for (auto kv: attributes) {
        kv.second->deserialize(element, kv.first);
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

}