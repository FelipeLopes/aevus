#include "flame.hpp"
#include <memory>
#include <sstream>

namespace gasket {

using std::string;

Flame::Flame(int width_, int height_, Palette palette_):
    palette(palette_), width(width_), height(height_) {

}

tinyxml2::XMLNode* Flame::toXMLNode(tinyxml2::XMLDocument& xmlDoc) {
    auto root = xmlDoc.NewElement("Flames");
    root->SetAttribute("name", "gasket");

    std::ostringstream size;
    size<<width<<" "<<height;

    auto flame = xmlDoc.NewElement("flame");
    flame->SetAttribute("name", "Gasket");
    flame->SetAttribute("background", "0 0 0");
    flame->SetAttribute("version", "Aevus");
    flame->SetAttribute("size", size.str().c_str());
    flame->SetAttribute("center", "0 0");
    flame->SetAttribute("scale", height/2);
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

void Flame::readXFormDistribution(render::XFormDistribution& dist) {
    dist.numXForms = xforms.size();
    int sizeGrains = render::XFormDistribution::XFORM_DISTRIBUTION_GRAINS;
    dist.data.resize(sizeGrains*dist.numXForms);
    for (int i=0; i<dist.numXForms; i++) {
        double acc = 0;
        std::vector<double> densities;
        for (int j=0; j<dist.numXForms; j++) {
            acc += xforms[i].chaos[j]*xforms[j].weight;
            densities.push_back(acc);
        }
        double step = acc / sizeGrains;
        int j = 0;
        double curr = 0;
        for (int k=0; k<sizeGrains; k++) {
            while (curr >= densities[j] && j<dist.numXForms) {
                j++;
            }
            dist.data[i*sizeGrains+k] = j;
            curr += step;
        }
    }
}

}