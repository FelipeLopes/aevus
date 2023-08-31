#pragma once

#include <memory>
#include "../core/flame.hpp"
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
    TriangleModel(std::shared_ptr<core::Flame> flame, wxPanel* trianglePanel);
    void update();
    void handleActiveXformChanged(int id);
    void handlePaint();
    void handleResize(wxSizeEvent& event);
    void handleMouseWheel(wxMouseEvent& event);
    void handleMouseUp(wxMouseEvent& event);
    void handleMouseDown(wxMouseEvent& event);
    void handleMouseMove(wxMouseEvent& event);

    boost::signals2::signal<void (int)> xformSelected;
    boost::signals2::signal<void ()> transformCoordsChanged;
private:
    std::shared_ptr<core::Flame> flame;
    std::shared_ptr<TriangleGrid> triangleGrid;
    std::shared_ptr<TriangleCollider> triangleCollider;
    std::shared_ptr<TriangleDrawer> triangleDrawer;
    std::shared_ptr<TriangleUpdater> triangleUpdater;
    wxPanel* trianglePanel;
    int activeTransform;
};

}