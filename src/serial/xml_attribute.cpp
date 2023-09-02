#include "xml_attribute.hpp"
#include "xml_element.hpp"

using boost::assign::list_of;
using boost::assign::map_list_of;
using std::function;
using std::map;
using std::set;
using std::string;
using std::to_string;
using tinyxml2::XMLElement;

namespace serial {

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

int XMLAttributeInt::value() const {
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
    return map_list_of(name, formattedDouble(val));
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

double XMLAttributeDouble::value() const {
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

string XMLAttributeString::value() const {
    return val;
}

void XMLAttributeString::setValue(string value) {
    val = value;
}

}