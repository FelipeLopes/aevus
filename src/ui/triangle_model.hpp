#pragma once

#include <memory>
#include "../core/flame.hpp"
#include <wx/graphics.h>
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
    void strokeLine(wxGraphicsContext* gc, double x1, double y1, double x2, double y2);

    std::shared_ptr<core::Flame> flame;
    wxPanel* trianglePanel;
    wxAffineMatrix2D affineTransform;
    wxColour gridColor, unitTriangleColor;
    double gridLowX, gridHighX;
    double gridLowY, gridHighY;
};

}