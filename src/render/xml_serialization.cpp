#include "xml_serialization.hpp"
#include <string>

using std::string;
using std::to_string;

namespace render {

XMLAttributeField::XMLAttributeField(XMLElementClass& element, string name) {
    element.attributes[name] = this;
}

XMLAttributeInt::XMLAttributeInt(XMLElementClass& element, string name):
    XMLAttributeField(element, name) { }

string XMLAttributeInt::value() {
    return to_string(val);
}

XMLAttributeDouble::XMLAttributeDouble(XMLElementClass& element, string name):
    XMLAttributeField(element, name) { }

string XMLAttributeDouble::value() {
    return to_string(val);
}

XMLAttributeString::XMLAttributeString(XMLElementClass& element, string name):
    XMLAttributeField(element, name) { }

string XMLAttributeString::value() {
    return val;
}

XMLContentString::XMLContentString(XMLElementClass& element) {
    element.contentString = this;
}

XMLElementClass::XMLElementClass(std::string tag_): tag(tag_) {

}

XMLElementClass::XMLElementClass(XMLElementClass& element, std::string tag_): tag(tag_) {
    element.children.push_back(this);
}

}