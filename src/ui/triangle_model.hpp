#pragma once

#include <memory>
#include "../core/flame.hpp"
#include <wx-3.2/wx/geometry.h>
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
    void handleMouseWheel(wxMouseEvent& event);
private:
    void setupAffine();
    void strokeLine(wxGraphicsContext* gc, double x1, double y1, double x2, double y2);
    void strokeLines(wxGraphicsContext* gc, const std::vector<wxPoint2DDouble>& arr);

    std::shared_ptr<core::Flame> flame;
    wxPanel* trianglePanel;
    wxAffineMatrix2D affineTransform;
    wxColour gridColor, unitTriangleColor;
    double gridLowX, gridHighX;
    double gridLowY, gridHighY;
    int zoomLevel;
    const double zoomFactor;
    static const int MAX_ZOOM_LEVEL = 400;
};

}