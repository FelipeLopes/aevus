#pragma once

#include <memory>
#include "../core/flame.hpp"
#include "content.hpp"
#include "triangle_collider.hpp"
#include "triangle_drawer.hpp"
#include "triangle_grid.hpp"
#include "triangle_types.hpp"
#include "triangle_updater.hpp"
#include <boost/signals2.hpp>
#include <wx/panel.h>

namespace ui {

class TriangleModel {
public:
    TriangleModel(wxPanel* trianglePanel);
    void update();
    void handlePaint();
    void handleResize(wxSizeEvent& event);
    void handleMouseWheel(wxMouseEvent& event);
    void handleMouseUp(wxMouseEvent& event);
    void handleMouseDown(wxMouseEvent& event);
    void handleMouseMove(wxMouseEvent& event);
    void handleFlameContent(std::optional<FlameContent> content);
    void handleContent(const XFormTriangleContent& content);

    boost::signals2::signal<void (int)> xformSelected;
    boost::signals2::signal<void (CoefsContent)> xformCoefsChanged;

    static std::array<std::pair<double, double>, 3> triangle(CoefsContent coefs);
private:
    wxPanel* trianglePanel;
    int activeId;
    TriangleGrid triangleGrid;
    TriangleCollider triangleCollider;
    TriangleDrawer triangleDrawer;
    TriangleUpdater triangleUpdater;
};

}