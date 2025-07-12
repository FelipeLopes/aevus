#pragma once

#include <iostream>
#include <wx/docview.h>
#include "../core/flame.hpp"

namespace ui {

class FlameDocument: public wxDocument {
public:
    std::ostream& SaveObject(std::ostream &stream) override;
    std::istream& LoadObject(std::istream &stream) override;
    bool flameHasXForms();
    void setFlame(core::Flame* flame);
    core::Flame* flame;

    wxDECLARE_DYNAMIC_CLASS(FlameDocument);
};

}