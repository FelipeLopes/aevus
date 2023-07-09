#include "xml_serialization.hpp"
#include <exception>
#include <stdexcept>
#include <string>

using std::string;
using std::to_string;
using tinyxml2::XMLDocument;
using tinyxml2::XMLElement;
using tinyxml2::XMLNode;

namespace render {

XMLAttributeField::XMLAttributeField(XMLElementClass& element, string name) {
    element.attributes[name] = this;
}

XMLAttributeInt::XMLAttributeInt(XMLElementClass& element, string name):
    XMLAttributeField(element, name) { }

string XMLAttributeInt::text() {
    return to_string(val);
}

int XMLAttributeInt::getValue() {
    return val;
}

void XMLAttributeInt::setValue(int value) {
    val = value;
}

XMLAttributeDouble::XMLAttributeDouble(XMLElementClass& element, string name):
    XMLAttributeField(element, name) { }

string XMLAttributeDouble::text() {
    return to_string(val);
}

double XMLAttributeDouble::getValue() {
    return val;
}

void XMLAttributeDouble::setValue(double value) {
    val = value;
}

XMLAttributeString::XMLAttributeString(XMLElementClass& element, string name):
    XMLAttributeField(element, name) { }

string XMLAttributeString::text() {
    return val;
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
        element->SetAttribute(kv.first.c_str(), kv.second->text().c_str());
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
    XMLNode* childNode = node->FirstChild();
    for (auto child: children) {
        child->nodeDeserialize(childNode);
        childNode = childNode->NextSibling();
    }
    if (contentString == NULL && childNode != NULL) {
        throw std::invalid_argument("XML node has incorrect number of children");
    }
}

}