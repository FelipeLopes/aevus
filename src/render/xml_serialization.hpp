#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>
#include <tinyxml2.h>

namespace render {

class XMLElementClass;

class XMLAttributeField {
public:
    XMLAttributeField(XMLElementClass& element, std::string name);
    virtual std::string text() = 0;
    virtual ~XMLAttributeField() { }
};

class XMLAttributeInt : public XMLAttributeField {
public:
    XMLAttributeInt(XMLElementClass& element, std::string name);
    virtual std::string text();
    int getValue();
    void setValue(int value);
private:
    int val;
};

class XMLAttributeDouble : public XMLAttributeField {
public:
    XMLAttributeDouble(XMLElementClass& element, std::string name);
    virtual std::string text();
    double getValue();
    void setValue(double value);
private:
    double val;
};

class XMLAttributeString : public XMLAttributeField {
public:
    XMLAttributeString(XMLElementClass& element, std::string name);
    virtual std::string text();
    std::string getValue();
    void setValue(std::string value);
private:
    std::string val;
};

class XMLContentString {
public:
    XMLContentString();
    XMLContentString(XMLElementClass& element);
    std::string getValue();
    void setValue(std::string value);
private:
    std::string val;
};

class XMLElementClass {
public:
    XMLElementClass(std::string tag);
    XMLElementClass(XMLElementClass& element, std::string tag);
    tinyxml2::XMLNode* serialize(tinyxml2::XMLDocument& xmlDoc);
    std::string tag;
    std::vector<XMLElementClass*> children;
    std::map<std::string, XMLAttributeField*> attributes;
    XMLContentString* contentString;
};

}