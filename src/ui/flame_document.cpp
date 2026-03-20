#include "flame_document.hpp"
#include <tinyxml2.h>
#include <wx/docview.h>

namespace ui {

wxIMPLEMENT_DYNAMIC_CLASS(FlameDocument, wxDocument);

bool FlameDocument::OnCreate(const wxString& path, long flags) {
    documentFlags = flags;
    if (!wxDocument::OnCreate(path, flags)){
        return false;
    }
    return true;
}

bool FlameDocument::isNew() {
    return documentFlags & wxDOC_NEW;
}

std::ostream& FlameDocument::SaveObject(std::ostream& stream) {
    flame.serialize(stream);
    stream.clear();
    return stream;
}

std::istream& FlameDocument::LoadObject(std::istream& stream) {
    tinyxml2::XMLDocument xmlDoc;
    std::ostringstream oss;
    stream >> oss.rdbuf();
    std::string s = oss.str();
    auto err = xmlDoc.Parse(s.c_str(), s.size());
    if (err != tinyxml2::XML_SUCCESS) {
        auto ec = std::error_code(err, std::generic_category());
        throw std::system_error(ec, "Could not load XML file");
    }
    tinyxml2::XMLNode* node = xmlDoc.FirstChild();
    core::XmlDeserializer deserializer(node);
    flameV.acceptDeserializer(deserializer);
    // flame.deserialize(stream);
    // workaround for wxWidgets problem: https://github.com/wxWidgets/wxWidgets/issues/23479
    stream.clear();
    printf("%lf %lf\n", flameV.center.x, flameV.center.y);
    printf("%d %d\n", flameV.size.width, flameV.size.height);
    printf("%lf\n", flameV.scale);
    printf("%s\n",flameV.name.c_str());
    //dynamic_cast<FlameView*>(GetFirstView())->documentLoaded();
    printf("%zu\n", flameV.xforms.size());
    printf("%lf %lf\n", flameV.xforms[0].coefs.ox, flameV.xforms[0].coefs.oy);
    printf("%lf\n",flameV.xforms[0].variationMap.variations[core::Variation::VariationID::LINEAR].weight);
    printf("%lf\n",flameV.finalXForm->color);
    return stream;
}

bool FlameDocument::flameHasXForms() {
    return !flame.xforms.empty();
}

}