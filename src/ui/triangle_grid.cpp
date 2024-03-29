#include "triangle_grid.hpp"
#include "triangle_types.hpp"
#include <map>
#include <vector>
#include <wx/pen.h>

using std::map;
using std::vector;

namespace ui {

TriangleGrid::TriangleGrid(wxSize size): center(0,0), width(size.GetWidth()),
    height(size.GetHeight()), gridColor("#333333"), unitTriangleColor("#808080"),
    zoomLevel(0), zoomFactor(1.1)
{
    updateWindowSize(width, height);
}

void TriangleGrid::strokeLine(wxGraphicsContext* gc, GridPoint p1, GridPoint p2) {
    // We implement our own strokeLine wrapper that accesses the affine
    // transform directly instead of using the builtin wxWidgets constructions.
    // The reason for this is that when stroking a line, the minimum pen width is 1,
    // and wxWidgets scales this width in the final custom control, making their
    // builtin methods useless for our purposes.
    auto tp1 = transformToWindow(p1);
    auto tp2 = transformToWindow(p2);
    gc->StrokeLine(tp1.m_x, tp1.m_y, tp2.m_x, tp2.m_y);
}

void TriangleGrid::strokeLines(wxGraphicsContext* gc, const std::vector<GridPoint>& arr) {
    vector<WindowPoint> transformedArr;
    std::transform(arr.begin(), arr.end(), std::back_inserter(transformedArr),
        [this](GridPoint pt) {
            return transformToWindow(pt);
        });
    gc->StrokeLines(transformedArr.size(), transformedArr.data());
}

void TriangleGrid::drawGrid(wxGraphicsContext* gc) {
    gc->SetPen(gridColor);
    int nx = (gridHighX - gridLowX)/step;
    int ny = (gridHighY - gridLowY)/step;
    for (int i=0; i<=nx; i++) {
        double x = gridLowX + step*i;
        strokeLine(gc, GridPoint(x, gridLowY), GridPoint(x, gridHighY));
    }
    for (int i=0; i<=ny; i++) {
        double y = gridLowY + step*i;
        strokeLine(gc, GridPoint(gridLowX, y), GridPoint(gridHighX, y));
    }
    gc->SetPen(wxPen(unitTriangleColor, 1, wxPENSTYLE_SHORT_DASH));
    vector<GridPoint> unitTriangle = {
        {0, 0}, {1, 0}, {0, 1}, {0, 0}
    };
    strokeLines(gc, unitTriangle);
}

void TriangleGrid::updateWindowSize(int width_, int height_) {
    width = width_;
    height = height_;
    updateGrid();
}

void TriangleGrid::updateCenter(GridPoint center_) {
    center = center_;
    updateGrid();
}

GridPoint TriangleGrid::transformToGrid(WindowPoint windowPoint) {
    wxAffineMatrix2D inverseAffine = affineTransform;
    inverseAffine.Invert();
    return GridPoint(inverseAffine.TransformPoint(windowPoint));
}

WindowPoint TriangleGrid::transformToWindow(GridPoint gridPoint) {
    return WindowPoint(affineTransform.TransformPoint(gridPoint));
}

void TriangleGrid::zoomIn() {
    if (zoomLevel < MAX_ZOOM_LEVEL) {
        zoomLevel++;
        updateGrid();
    }
}

void TriangleGrid::zoomOut() {
    if (zoomLevel > -MAX_ZOOM_LEVEL) {
        zoomLevel--;
        updateGrid();
    }
}

wxAffineMatrix2D TriangleGrid::getAffineMatrix() {
    return affineTransform;
}

wxPoint2DDouble TriangleGrid::getCenter() {
    return center;
}

void TriangleGrid::updateGrid() {
    wxMatrix2D identity;
    wxPoint2DDouble origin;
    affineTransform.Set(identity, origin);
    affineTransform.Translate(width/2.0, height/2.0);
    double sc = pow(zoomFactor, zoomLevel);
    affineTransform.Scale(50*sc, -50*sc);
    step = calcStepForScale(sc);
    affineTransform.Translate(-center.m_x, -center.m_y);
    gridLowX = step*floor((-width/(2*sc) + center.m_x)/step);
    gridHighX = step*ceil((width/(2*sc) + center.m_x)/step);
    gridLowY = step*floor((-height/(2*sc) + center.m_y)/step);
    gridHighY = step*ceil((height/(2*sc) + center.m_y)/step);
}

double TriangleGrid::calcStepForScale(double sc) {
    map<double, double> multiplier {
        {0.4, 1.0},
        {0.7, 0.5},
        {0.9, 0.2},
        {1.0, 0.1}
    };
    double logarithm = log10(sc);
    double periodBegin = floor(logarithm);
    double frac = logarithm - periodBegin;
    return pow(10, -periodBegin)*multiplier.lower_bound(frac)->second;
}

}