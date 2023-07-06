#include "xml_serialization.hpp"
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

XMLNode* XMLElementClass::serialize(XMLDocument& xmlDoc) {
    XMLElement* element = xmlDoc.NewElement(tag.c_str());
    for (auto kv: attributes) {
        element->SetAttribute(kv.first.c_str(), kv.second->text().c_str());
    }
    for (auto child: children) {
        element->InsertEndChild(child->serialize(xmlDoc));
    }
    if (contentString != NULL) {
        element->SetText(contentString->getValue().c_str());
    }
    return element;
}

}