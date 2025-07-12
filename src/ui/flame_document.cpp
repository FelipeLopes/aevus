#include "flame_document.hpp"
#include "flame_view.hpp"
#include <wx/docview.h>

namespace ui {

wxIMPLEMENT_DYNAMIC_CLASS(FlameDocument, wxDocument);

std::ostream& FlameDocument::SaveObject(std::ostream& stream) {
    return stream;
}

std::istream& FlameDocument::LoadObject(std::istream& stream) {
    flame.deserialize(stream);
     // workaround for wxWidgets problem: https://github.com/wxWidgets/wxWidgets/issues/23479
    stream.clear();
    dynamic_cast<FlameView*>(GetFirstView())->documentLoaded();
    return stream;
}

bool FlameDocument::flameHasXForms() {
    return !flame.xforms.empty();
}

}