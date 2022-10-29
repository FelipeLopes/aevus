#include "flame.h"

using std::string;

Flame::Flame() {

}

tinyxml2::XMLNode* Flame::toXMLNode(tinyxml2::XMLDocument& xmlDoc) {
    auto root = xmlDoc.NewElement("Flames");
    root->SetAttribute("name", "gasket");

    auto flame = xmlDoc.NewElement("flame");
    flame->SetAttribute("name", "Gasket");
    flame->SetAttribute("background", "0 0 0");
    flame->SetAttribute("version", "Apophysis 2.09");
    flame->SetAttribute("size", "600 600");
    flame->SetAttribute("center", "0 0");
    flame->SetAttribute("scale", "144");
    flame->SetAttribute("oversample", "1");
    flame->SetAttribute("filter", "0.2");
    flame->SetAttribute("quality", "1");
    flame->SetAttribute("background", "0 0 0");
    flame->SetAttribute("brightness", "4");
    flame->SetAttribute("gamma", "4");
    flame->SetAttribute("gamma_threshold", "0.04");
    root->InsertEndChild(flame);

    for (auto xf: xforms) {
        flame->InsertEndChild(xf.toXMLNode(xmlDoc));
    }

    auto paletteElement = xmlDoc.NewElement("palette");
    paletteElement->SetAttribute("count", "256");
    paletteElement->SetAttribute("format", "RGB");
    string whiteSpace(12, ' ');
    string text = "\n";
    for (int i=0; i<32; i++) {
        string paletteChars = "";
        for (int j=0; j<8; j++) {
            paletteChars += palette.hexAt(i*8+j);
        }
        text += (whiteSpace + paletteChars + "\n");
    }
    text += "        ";
    paletteElement->SetText(text.c_str());
    flame->InsertEndChild(paletteElement);

    return root;
}