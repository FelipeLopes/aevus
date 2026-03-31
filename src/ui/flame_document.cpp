#include "flame_document.hpp"
#include "commands.hpp"
#include "flame_view.hpp"
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
    tinyxml2::XMLDocument xmlDoc;
    core::XmlSerializer serializer(&xmlDoc, &xmlDoc);
    flame.acceptSerializer(serializer);
    tinyxml2::XMLPrinter xmlPrinter;
    xmlDoc.Print(&xmlPrinter);
    stream << xmlPrinter.CStr();
    stream.clear();
    GetCommandProcessor()->MarkAsSaved();
    Modify(false);
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
    flame.acceptDeserializer(deserializer);
    // flame.deserialize(stream);
    // workaround for wxWidgets problem: https://github.com/wxWidgets/wxWidgets/issues/23479
    stream.clear();


    flame.gradient.colorStops.emplace_back(0, core::GradientColor(168.0/255, 1, 0));
    flame.gradient.colorStops.emplace_back(0.17, core::GradientColor(194.0/255, 56.0/255, 56.0/255));
    flame.gradient.colorStops.emplace_back(0.398, core::GradientColor(0, 0, 0));
    flame.gradient.colorStops.emplace_back(0.65, core::GradientColor(149.0/255, 149.0/255, 149.0/255));
    flame.gradient.colorStops.emplace_back(0.793, core::GradientColor(179.0/255, 202.0/255, 179.0/255));
    flame.gradient.colorStops.emplace_back(1, core::GradientColor(210.0/255, 1, 210.0/255));

    flame.gradient.opacityStops.emplace_back(0, 1.0);
    flame.gradient.opacityStops.emplace_back(1, 1.0);

    dynamic_cast<FlameView*>(GetFirstView())->documentLoaded();
    return stream;
}

wxCommandProcessor* FlameDocument::OnCreateCommandProcessor() {
    return new FlameCommandProcessor();
}

bool FlameDocument::flameHasXForms() {
    return !flame.xforms.empty();
}

}