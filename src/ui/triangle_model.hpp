#pragma once

#include <memory>
#include "../core/flame.hpp"
#include <wx/event.h>
#include <wx/affinematrix2d.h>
#include <wx/panel.h>

namespace ui {

class TriangleModel {
public:
    TriangleModel(std::shared_ptr<core::Flame> flame, wxPanel* trianglePanel);
    void handlePaint();
    void handleResize(wxSizeEvent& event);
private:
    std::shared_ptr<core::Flame> flame;
    wxPanel* trianglePanel;
    wxAffineMatrix2D affineTransform;
};

}