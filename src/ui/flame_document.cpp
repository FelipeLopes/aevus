#include "flame_document.hpp"

wxIMPLEMENT_DYNAMIC_CLASS(FlameDocument, wxDocument);

std::ostream& FlameDocument::SaveObject(std::ostream& stream) {
    return stream;
}

std::istream& FlameDocument::LoadObject(std::istream& stream) {
    return stream;
}