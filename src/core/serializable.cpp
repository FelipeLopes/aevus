#include "serializable.hpp"
#include <string>
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

}