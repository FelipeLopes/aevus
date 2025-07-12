#include "xml_element.hpp"
#include "xml_attribute.hpp"
#include "xml_content.hpp"
#include <sstream>

using std::string;
using tinyxml2::XMLDocument;
using tinyxml2::XMLElement;
using tinyxml2::XMLNode;

namespace serial {

XMLElementClass::XMLElementClass(string tag_): tag(tag_) {
    content = NULL;
}

XMLElementClass::XMLElementClass(XMLElementClass& parent, string tag_): tag(tag_) {
    content = NULL;
    parent.children.push_back(this);
}

void XMLElementClass::serialize(FILE* fp) {
    XMLDocument xmlDoc;
    nodeSerialize(xmlDoc, &xmlDoc);
    xmlDoc.SaveFile(fp);
}

void XMLElementClass::deserialize(std::istream& stream) {
    XMLDocument xmlDoc;
    std::ostringstream oss;
    stream >> oss.rdbuf();
    string s = oss.str();
    auto err = xmlDoc.Parse(s.c_str(), s.size());
    if (err != tinyxml2::XML_SUCCESS) {
        auto ec = std::error_code(err, std::generic_category());
        throw std::system_error(ec, "Could not load XML file");
    }
    XMLNode* node = xmlDoc.FirstChild();
    node = nodeDeserialize(node);
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
    if (content != NULL) {
        element->SetText(content->serialize().c_str());
    }
    parent->InsertEndChild(element);
}

XMLNode* XMLElementClass::nodeDeserialize(XMLNode* node) {
    if (node == NULL) {
        throw std::invalid_argument("XML document has no nodes");
    }
    XMLElement* element = node->ToElement();
    if (element == NULL) {
        throw std::invalid_argument("XML node is not an element");
    }
    string name = element->Name();
    if (name != tag) {
        throw std::invalid_argument("XML node has invalid tag: " + name);
    }
    for (auto field: attributeFields) {
        field->deserialize(element);
    }
    XMLNode* childNode = node->FirstChild();
    for (auto child: children) {
        childNode = child->nodeDeserialize(childNode);
    }
    if (content == NULL && childNode != NULL) {
        throw std::invalid_argument("XML node has incorrect number of children");
    }
    if (content != NULL) {
        if (childNode == NULL) {
            throw std::invalid_argument("XML node has no content");
        }
        content->deserialize(childNode);
    }
    return node->NextSibling();
}

XMLElementClass::~XMLElementClass() { }

}