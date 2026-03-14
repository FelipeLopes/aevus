#include "serializable.hpp"
#include <stdexcept>
#include <string>
#include <stdlib.h>
#include <tinyxml2.h>
#include "flame.hpp"

using std::string;
using std::to_string;
using tinyxml2::XMLDocument;
using tinyxml2::XMLElement;
using tinyxml2::XMLNode;

namespace core {

XmlSerializer::XmlSerializer(XMLDocument* xmlDoc_, XMLNode* parent_): xmlDoc(xmlDoc_), parent(parent_) { }

void XmlSerializer::serialize(FlameV& flame) {
    XMLElement* element = xmlDoc->NewElement("flame");
    element->SetAttribute("version", flame.version.c_str());
    element->SetAttribute("name", flame.name.c_str());
    string size = to_string(flame.size.width) + " " + to_string(flame.size.height);
    element->SetAttribute("size", size.c_str());
    string center = serial::formattedDouble(flame.center.x) + " " + serial::formattedDouble(flame.center.y);
    element->SetAttribute("center", center.c_str());
    element->SetAttribute("scale", serial::formattedDouble(flame.scale).c_str());
    element->SetAttribute("quality", serial::formattedDouble(flame.quality).c_str());
    string color = to_string(flame.background.r) + " " + to_string(flame.background.g) + " " + to_string(flame.background.b);
    element->SetAttribute("background", color.c_str());
    element->SetAttribute("brightness", serial::formattedDouble(flame.brightness).c_str());
    element->SetAttribute("contrast", serial::formattedDouble(flame.contrast).c_str());
    element->SetAttribute("gamma", serial::formattedDouble(flame.gamma).c_str());
    element->SetAttribute("vibrancy", serial::formattedDouble(flame.vibrancy).c_str());
    switch (flame.clippingMode) {
        case ALPHA: element->SetAttribute("clipping", "alpha"); break;
        case CHANNEL: element->SetAttribute("clipping", "channel"); break;
        case WHITE: element->SetAttribute("clipping", "white"); break;
    }
    auto childSerializer = XmlSerializer(xmlDoc, element);
    for (auto xf: flame.xforms) {
        childSerializer.serialize(xf);
    }
    if (flame.finalXForm.has_value()) {
        childSerializer.serialize(flame.finalXForm.value());
    }
    if (parent != NULL) {
        parent->InsertEndChild(element);
    }
}

void XmlSerializer::serializeBaseXForm(BaseXFormV& xform, XMLElement* element) {
    element->SetAttribute("color", xform.color);
    element->SetAttribute("color_speed", xform.colorSpeed);
    auto stringMap = xform.variationMap.toStringMap();
    for (auto kv: stringMap) {
        element->SetAttribute(kv.first.c_str(), kv.second.c_str());
    }
    auto coefsString = xform.coefs.toString();
    if (coefsString.has_value()) {
        element->SetAttribute("coefs", coefsString.value().c_str());
    }
    auto postString = xform.post.toString();
    if (postString.has_value()) {
        element->SetAttribute("post", postString.value().c_str());
    }
    auto chaosString = xform.chaos.toString();
    if (chaosString.has_value()) {
        element->SetAttribute("chaos", chaosString.value().c_str());
    }
}

void XmlSerializer::serialize(XFormV& xform) {
    XMLElement* element = xmlDoc->NewElement("xform");
    serializeBaseXForm(xform, element);
    element->SetAttribute("weight", xform.weight);
    if (parent != NULL) {
        parent->InsertEndChild(element);
    }
}

void XmlSerializer::serialize(FinalXFormV& finalXform) {
    XMLElement* element = xmlDoc->NewElement("finalxform");
    serializeBaseXForm(finalXform, element);
    if (parent != NULL) {
        parent->InsertEndChild(element);
    }
}

void XmlSerializer::serialize(PaletteV& palette) {
    XMLElement* element = xmlDoc->NewElement("palette");
    element->SetAttribute("count", palette.count);
    element->SetAttribute("format", palette.format.c_str());
    // TODO: change return type to string instead of optional
    element->SetText(palette.colors.toString()->c_str());
    if (parent != NULL) {
        parent->InsertEndChild(element);
    }
}

XmlDeserializer::XmlDeserializer(XMLNode* parent_): parent(parent_) { }

void XmlDeserializer::deserialize(FlameV& flame) {
    if (parent == NULL) {
        throw std::invalid_argument("Flame node is null");
    }
    XMLElement* element = parent->ToElement();
    if (element == NULL) {
        throw std::invalid_argument("Flame node is not an XML element");
    }
    string name = element->Name();
    if (name != "flame") {
        throw std::invalid_argument("Flame node has incorrect tag: "+name);
    }
    const char* buf;
    auto err = element->QueryStringAttribute("version", &buf);
    if (err == tinyxml2::XML_SUCCESS) {
        flame.version = buf;
        free((void*)buf);
    }
    err = element->QueryStringAttribute("name", &buf);
    if (err == tinyxml2::XML_SUCCESS) {
        flame.name = buf;
        free((void*)buf);
    }
    err = element->QueryStringAttribute("size", &buf);
    if (err != tinyxml2::XML_SUCCESS) {
        throw std::invalid_argument("Could not read flame size");
    }
    flame.size.fromString(buf);
    free((void*)buf);
    err = element->QueryStringAttribute("center", &buf);
    if (err != tinyxml2::XML_SUCCESS) {
        throw std::invalid_argument("Could not read flame center");
    }
    flame.center.fromString(buf);
    free((void*)buf);
    err = element->QueryDoubleAttribute("scale", &flame.scale);
    if (err != tinyxml2::XML_SUCCESS) {
        throw std::invalid_argument("Could not read flame scale");
    }
    err = element->QueryDoubleAttribute("quality", &flame.quality);
    if (err != tinyxml2::XML_SUCCESS) {
        throw std::invalid_argument("Could not read flame quality");
    }
    err = element->QueryStringAttribute("background", &buf);
    if (err != tinyxml2::XML_SUCCESS) {
        throw std::invalid_argument("Could not read flame background");
    }
    flame.background.fromString(buf);
    free((void*)buf);
    flame.brightness = element->DoubleAttribute("brightness", 4.0);
    flame.quality = element->DoubleAttribute("quality", 5.0);
    flame.contrast = element->DoubleAttribute("contrast", 1.0);
    flame.gamma = element->DoubleAttribute("gamma", 4.0);
    flame.vibrancy = element->DoubleAttribute("vibrancy", 1.0);
    element->QueryStringAttribute("clipping", &buf);
    if (err != tinyxml2::XML_SUCCESS) {
        flame.clippingMode = WHITE;
    } else {
        string clipping = buf;
        if (clipping == "alpha") {
            flame.clippingMode = ALPHA;
        } else if (clipping == "channel") {
            flame.clippingMode = CHANNEL;
        } else {
            flame.clippingMode = WHITE;
        }
        free((void*)buf);
    }
}

}