#include "xml_serialization.hpp"
#include <string>

using std::string;
using std::to_string;

namespace render {

XMLAttributeField::XMLAttributeField(XMLElementClass& element, string name) {

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
    element.contentString = val;
}

XMLElementClass::XMLElementClass(std::string tag_): tag(tag_) {

}

}