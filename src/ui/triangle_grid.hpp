#pragma once

#include <wx/affinematrix2d.h>
#include <wx/colour.h>
#include <wx/geometry.h>
#include <wx/graphics.h>

namespace ui {

class TriangleGrid {
public:
    TriangleGrid(int width, int height);
    void strokeLine(wxGraphicsContext* gc, double x1, double y1, double x2, double y2);
    void strokeLines(wxGraphicsContext* gc, const std::vector<wxPoint2DDouble>& arr);
    void drawGrid(wxGraphicsContext* gc);
    void updateWindowSize(int width, int height);
    void updateCenter(wxPoint2DDouble center);
    wxPoint2DDouble transformToGrid(wxPoint2DDouble windowPoint);
    wxPoint2DDouble transformToWindow(wxPoint2DDouble gridPoint);
    void zoomIn();
    void zoomOut();
    wxAffineMatrix2D getAffineMatrix();
    wxPoint2DDouble getCenter();
private:
    void updateGrid();
    double calcStepForScale(double sc);

    wxAffineMatrix2D affineTransform;
    wxPoint2DDouble center;
    int width, height;
    wxColour gridColor, unitTriangleColor;
    double gridLowX, gridHighX;
    double gridLowY, gridHighY;
    double step;
    int zoomLevel;
    const double zoomFactor;
    static const int MAX_ZOOM_LEVEL = 400;
};

}