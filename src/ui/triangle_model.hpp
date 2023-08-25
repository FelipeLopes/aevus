#pragma once

#include <memory>
#include "../core/flame.hpp"
#include <wx/panel.h>

namespace ui {

class TriangleModel {
public:
    TriangleModel(std::shared_ptr<core::Flame> flame, wxPanel* trianglePanel);
    void handlePaint();
private:
    std::shared_ptr<core::Flame> flame;
    wxPanel* trianglePanel;
};

}