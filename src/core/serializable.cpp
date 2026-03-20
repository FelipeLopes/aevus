#include "serializable.hpp"
#include <iomanip>
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

string formattedDouble(double x) {
    std::stringstream buffer;
    buffer<<std::setprecision(15)<<x;
    auto ans = buffer.str();
    if (ans == "-0") {
        ans = "0";
    }
    return ans;
}

XmlSerializer::XmlSerializer(XMLDocument* xmlDoc_, XMLNode* parent_): xmlDoc(xmlDoc_), parent(parent_) { }

void XmlSerializer::serialize(Flame& flame) {
    XMLElement* element = xmlDoc->NewElement("flame");
    element->SetAttribute("version", flame.version.c_str());
    element->SetAttribute("name", flame.name.c_str());
    string size = to_string(flame.size.width) + " " + to_string(flame.size.height);
    element->SetAttribute("size", size.c_str());
    string center = formattedDouble(flame.center.x) + " " + formattedDouble(flame.center.y);
    element->SetAttribute("center", center.c_str());
    element->SetAttribute("scale", formattedDouble(flame.scale).c_str());
    element->SetAttribute("quality", formattedDouble(flame.quality).c_str());
    string color = to_string(flame.background.r) + " " + to_string(flame.background.g) + " " + to_string(flame.background.b);
    element->SetAttribute("background", color.c_str());
    element->SetAttribute("brightness", formattedDouble(flame.brightness).c_str());
    element->SetAttribute("contrast", formattedDouble(flame.contrast).c_str());
    element->SetAttribute("gamma", formattedDouble(flame.gamma).c_str());
    element->SetAttribute("vibrancy", formattedDouble(flame.vibrancy).c_str());
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
    childSerializer.serialize(flame.palette);
    parent->InsertEndChild(element);
}

void XmlSerializer::serializeBaseXForm(BaseXForm& xform, XMLElement* element) {
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

void XmlSerializer::serialize(XForm& xform) {
    XMLElement* element = xmlDoc->NewElement("xform");
    serializeBaseXForm(xform, element);
    element->SetAttribute("weight", xform.weight);
    parent->InsertEndChild(element);
}

void XmlSerializer::serialize(FinalXForm& finalXform) {
    XMLElement* element = xmlDoc->NewElement("finalxform");
    serializeBaseXForm(finalXform, element);
    parent->InsertEndChild(element);
}

void XmlSerializer::serialize(Palette& palette) {
    XMLElement* element = xmlDoc->NewElement("palette");
    element->SetAttribute("count", palette.count);
    element->SetAttribute("format", palette.format.c_str());
    // TODO: change return type to string instead of optional
    element->SetText(palette.colors.toString()->c_str());
    parent->InsertEndChild(element);
}

XmlDeserializer::XmlDeserializer(XMLNode* parent_): parent(parent_) {
    for (auto kv: Variation::variationNames.right) {
        variationAttributeNames.insert(kv.first);
        auto paramNames = Variation::variationParamNames.find(kv.second)->second.paramNames;
        for (auto param: paramNames) {
            variationAttributeNames.insert(kv.first + "_" + param);
        }
    }
}

void XmlDeserializer::deserialize(Flame& flame) {
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
    }
    err = element->QueryStringAttribute("name", &buf);
    if (err == tinyxml2::XML_SUCCESS) {
        flame.name = buf;
    }
    err = element->QueryStringAttribute("size", &buf);
    if (err != tinyxml2::XML_SUCCESS) {
        throw std::invalid_argument("Could not read flame size");
    }
    flame.size.fromString(buf);
    err = element->QueryStringAttribute("center", &buf);
    if (err != tinyxml2::XML_SUCCESS) {
        throw std::invalid_argument("Could not read flame center");
    }
    flame.center.fromString(buf);
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
    }
    auto node = parent->FirstChild();
    if (node == NULL) {
        throw std::invalid_argument("Flame node has no children.");
    }
    element = node->ToElement();
    if (element == NULL) {
        throw std::invalid_argument("Child of flame node is not an XML element");
    }
    while (element->Name() == std::string("xform")) {
        flame.xforms.push_back(XForm());
        int idx = flame.xforms.size() - 1;
        auto xformDeserializer = XmlDeserializer(node);
        flame.xforms[idx].acceptDeserializer(xformDeserializer);
        node = node->NextSibling();
        element = node->ToElement();
        if (element == NULL) {
            throw std::invalid_argument("Child of flame node is not an XML element");
        }
    }
    element = node->ToElement();
    if (element == NULL) {
        throw std::invalid_argument("Child of flame node is not an XML element");
    }
    if (element->Name() == std::string("finalxform")) {
        auto finalXFormDeserializer = XmlDeserializer(node);
        flame.finalXForm = FinalXForm();
        flame.finalXForm->acceptDeserializer(finalXFormDeserializer);
        node = node->NextSibling();
    }
    auto paletteDeserializer = XmlDeserializer(node);
    flame.palette.acceptDeserializer(paletteDeserializer);
}

void XmlDeserializer::deserializeBaseXForm(BaseXForm& xform,  XMLElement* element) {
    auto err = element->QueryDoubleAttribute("color", &xform.color);
    if (err != tinyxml2::XML_SUCCESS) {
        throw std::invalid_argument("Could not read xform color");
    }
    err = element->QueryDoubleAttribute("color_speed", &xform.colorSpeed);
    if (err != tinyxml2::XML_SUCCESS) {
        err = element->QueryDoubleAttribute("symmetry", &xform.colorSpeed);
        if (err != tinyxml2::XML_SUCCESS) {
            xform.colorSpeed = 0.5;
        }
    }
    const char* buf;
    std::map<std::string, std::string> stringMap;
    for (auto name: variationAttributeNames) {
        auto err = element->QueryStringAttribute(name.c_str(), &buf);
        if (err == tinyxml2::XML_NO_ATTRIBUTE) {
            continue;
        }
        if (err != tinyxml2::XML_SUCCESS) {
            auto ec = std::error_code(err, std::generic_category());
            std::string msg("Could not load string attribute with name ");
            throw std::system_error(ec, msg + name);
        }
        stringMap[name] = buf;
    }
    xform.variationMap.fromStringMap(stringMap);
    err = element->QueryStringAttribute("coefs", &buf);
    if (err != tinyxml2::XML_SUCCESS) {
        throw std::invalid_argument("Could not read xform coefs");
    }
    xform.coefs.fromString(buf);
    err = element->QueryStringAttribute("post", &buf);
    if (err == tinyxml2::XML_SUCCESS) {
        xform.post.fromString(buf);
    }
    err = element->QueryStringAttribute("chaos", &buf);
    if (err == tinyxml2::XML_SUCCESS) {
        xform.chaos.fromString(buf);
    }
}

void XmlDeserializer::deserialize(XForm& xform) {
    if (parent == NULL) {
        throw std::invalid_argument("XForm node is null");
    }
    XMLElement* element = parent->ToElement();
    if (element == NULL) {
        throw std::invalid_argument("XForm node is not an XML element");
    }
    string name = element->Name();
    if (name != "xform") {
        throw std::invalid_argument("XForm node has incorrect tag: "+name);
    }
    deserializeBaseXForm(xform, element);
    auto err = element->QueryDoubleAttribute("weight", &xform.weight);
    if (err != tinyxml2::XML_SUCCESS) {
        throw std::invalid_argument("Could not read xform weight");
    }
}

void XmlDeserializer::deserialize(FinalXForm& finalXform) {
    if (parent == NULL) {
        throw std::invalid_argument("Final XForm node is null");
    }
    XMLElement* element = parent->ToElement();
    if (element == NULL) {
        throw std::invalid_argument("Final XForm node is not an XML element");
    }
    string name = element->Name();
    if (name != "finalxform") {
        throw std::invalid_argument("Final XForm node has incorrect tag: "+name);
    }
    deserializeBaseXForm(finalXform, element);
}

void XmlDeserializer::deserialize(Palette& palette) {
    if (parent == NULL) {
        throw std::invalid_argument("Palette node is null");
    }
    XMLElement* element = parent->ToElement();
    if (element == NULL) {
        throw std::invalid_argument("Palette node is not an XML element");
    }
    string name = element->Name();
    if (name != "palette") {
        throw std::invalid_argument("Palette node has incorrect tag: "+name);
    }
    element->QueryIntAttribute("count", &palette.count);
    const char* buf;
    element->QueryStringAttribute("format", &buf);
    palette.format = buf;
    auto text = element->GetText();
    if (text == NULL) {
        throw std::invalid_argument("Palette node has no text");
    }
    palette.colors.fromString(text);
}

}