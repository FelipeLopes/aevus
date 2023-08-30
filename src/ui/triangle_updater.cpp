#include "triangle_updater.hpp"

using core::Flame;
using std::shared_ptr;

namespace ui {

TriangleUpdater::TriangleUpdater(shared_ptr<Flame> flame_, shared_ptr<TriangleGrid> triangleGrid_):
    flame(flame_), triangleGrid(triangleGrid_), state(NO_UPDATE) { }

bool TriangleUpdater::updating() {
    return state != NO_UPDATE;
}

void TriangleUpdater::startGridDrag(WindowPoint clickPoint) {
    state = DRAGGING_GRID;
}

void TriangleUpdater::startTriangleDrag(WindowPoint clickPoint) {
    state = DRAGGING_TRIANGLE;
}

void TriangleUpdater::startXDrag(WindowPoint clickPoint) {
    state = DRAGGING_X;
}

void TriangleUpdater::startYDrag(WindowPoint clickPoint) {
    state = DRAGGING_Y;
}

void TriangleUpdater::startTriangleRotation(WindowPoint clickPoint) {
    state = ROTATING_TRIANGLE;
}

void TriangleUpdater::startTriangleScaling(WindowPoint clickPoint) {
    state = SCALING_TRIANGLE;
}

void TriangleUpdater::setUpdatePoint(WindowPoint mousePoint) {

}

void TriangleUpdater::finishUpdate() {
    state = NO_UPDATE;
}


}