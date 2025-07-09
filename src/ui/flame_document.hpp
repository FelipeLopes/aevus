#pragma once

#include <iostream>
#include <wx/docview.h>

class FlameDocument: public wxDocument {
public:
    std::ostream& SaveObject(std::ostream &stream) override;
    std::istream& LoadObject(std::istream &stream) override;

    wxDECLARE_DYNAMIC_CLASS(FlameDocument);
};