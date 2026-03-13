#include "serializable.hpp"
#include <string>
#include <tinyxml2.h>
#include "flame.hpp"

using std::string;
using std::to_string;
using tinyxml2::XMLDocument;
using tinyxml2::XMLElement;

namespace core {

XmlSerializer::XmlSerializer(XMLDocument* xmlDoc_): xmlDoc(xmlDoc_) { }

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
}

}