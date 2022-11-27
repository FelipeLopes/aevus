#include "flame.hpp"
#include <iomanip>
#include <memory>
#include <sstream>

namespace render {

using std::string;

Flame::Flame(double centerX_, double centerY_, int width_, int height_, Palette palette_):
    palette(palette_), centerX(centerX_), centerY(centerY_), scale(height_/2.0), width(width_),
    height(height_)
{

}

tinyxml2::XMLNode* Flame::toXMLNode(tinyxml2::XMLDocument& xmlDoc) {
    auto root = xmlDoc.NewElement("Flames");
    root->SetAttribute("name", "gasket");

    std::ostringstream center;
    center<<std::fixed<<std::setprecision(6)<<centerX<<" "<<centerY;
    std::ostringstream size;
    size<<width<<" "<<height;

    auto flame = xmlDoc.NewElement("flame");
    flame->SetAttribute("name", "Gasket");
    flame->SetAttribute("background", "0 0 0");
    flame->SetAttribute("version", "Aevus");
    flame->SetAttribute("size", size.str().c_str());
    flame->SetAttribute("center", center.str().c_str());
    flame->SetAttribute("scale", scale);
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

void Flame::readXFormCLArray(std::vector<render::XFormCL>& arr) const {
    arr.resize(xforms.size());
    for (int i=0; i<arr.size(); i++) {
        arr[i] = xforms[i].toXFormCL();
    }
}

void Flame::readXFormDistribution(std::vector<uint8_t>& dist) const {
    dist.resize(XFORM_DISTRIBUTION_GRAINS*xforms.size());
    for (int i=0; i<xforms.size(); i++) {
        double acc = 0;
        std::vector<double> densities;
        for (int j=0; j<xforms.size(); j++) {
            acc += xforms[i].chaos[j]*xforms[j].weight;
            densities.push_back(acc);
        }
        double step = acc / XFORM_DISTRIBUTION_GRAINS;
        int j = 0;
        double curr = 0;
        for (int k=0; k<XFORM_DISTRIBUTION_GRAINS; k++) {
            while (curr >= densities[j] && j<xforms.size()) {
                j++;
            }
            dist[i*XFORM_DISTRIBUTION_GRAINS+k] = j;
            curr += step;
        }
    }
}

render::FlameCL Flame::getFlameCL() const {
    render::FlameCL flameCL;
    flameCL.cx = centerX;
    flameCL.cy = centerY;
    flameCL.scale = scale;
    flameCL.width = width;
    flameCL.height = height;
    return flameCL;
}

}