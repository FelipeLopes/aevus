#include "xml_serialization.hpp"
#include <string>

using std::string;
using std::to_string;

namespace render {

XMLAttributeField::XMLAttributeField(XMLElementClass& element, std::string name) {

}

XMLAttributeInt::XMLAttributeInt(XMLElementClass& element, std::string name):
    XMLAttributeField(element, name) { }

string XMLAttributeInt::value() {
    return to_string(val);
}

XMLAttributeDouble::XMLAttributeDouble(XMLElementClass& element, std::string name):
    XMLAttributeField(element, name) { }

string XMLAttributeDouble::value() {
    return to_string(val);
}

XMLAttributeString::XMLAttributeString(XMLElementClass& element, std::string name):
    XMLAttributeField(element, name) { }

string XMLAttributeString::value() {
    return val;
}

XMLContentString::XMLContentString(XMLElementClass& element) {
    element.contentString = val;
}

XMLElementClass::XMLElementClass(std::string tag_): tag(tag_) {

}

}