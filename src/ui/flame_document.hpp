#pragma once

#include <iostream>
#include <wx/docview.h>
#include "../core/flame.hpp"

namespace ui {

class FlameDocument: public wxDocument {
public:
    bool OnCreate(const wxString& path, long flags) override;
    bool isNew();
    std::ostream& SaveObject(std::ostream &stream) override;
    std::istream& LoadObject(std::istream &stream) override;
    bool flameHasXForms();
    core::Flame flame;
private:
    long documentFlags;

    wxDECLARE_DYNAMIC_CLASS(FlameDocument);
};

}