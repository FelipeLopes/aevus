#include "xml_content.hpp"
#include <string>
#include <tinyxml2.h>
#include "xml_element.hpp"

using std::string;
using tinyxml2::XMLNode;

namespace serial {

XMLContentClass::XMLContentClass(XMLElementClass& parent) {
    parent.content = this;
}

XMLContentString::XMLContentString(XMLElementClass& parent): XMLContentClass(parent) { }

void XMLContentString::deserialize(XMLNode* node) {
    val = node->Value();
}

string XMLContentString::serialize() {
    return val;
}

void XMLContentString::setValue(string value) {
    val = value;
}

}