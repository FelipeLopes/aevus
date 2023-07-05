#pragma once

#include <deque>
#include <map>
#include <memory>
#include <string>

namespace render {

class XMLElementClass;

class XMLAttributeField {
public:
    XMLAttributeField(XMLElementClass& element, std::string name);
    virtual std::string value() = 0;
    virtual ~XMLAttributeField() { }
};

class XMLAttributeInt : public XMLAttributeField {
public:
    XMLAttributeInt(XMLElementClass& element, std::string name);
    virtual std::string value();
    int val;
};

class XMLAttributeDouble : public XMLAttributeField {
public:
    XMLAttributeDouble(XMLElementClass& element, std::string name);
    virtual std::string value();
    double val;
};

class XMLAttributeString : public XMLAttributeField {
public:
    XMLAttributeString(XMLElementClass& element, std::string name);
    virtual std::string value();
    std::string val;
};

class XMLContentString {
public:
    XMLContentString();
    XMLContentString(XMLElementClass& element);
    std::string val;
};

class XMLElementClass {
public:
    XMLElementClass(std::string tag);
    std::string contentString;
    std::string tag;
private:
    std::deque<XMLElementClass> children;
    std::map<std::string, std::unique_ptr<XMLAttributeField>> attributes;
};

}