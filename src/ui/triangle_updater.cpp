#include "triangle_updater.hpp"
#include "triangle_types.hpp"

using core::Flame;
using std::shared_ptr;

namespace ui {

TriangleUpdater::TriangleUpdater(shared_ptr<Flame> flame_, shared_ptr<TriangleGrid> triangleGrid_,
    int activeTransform_): flame(flame_), triangleGrid(triangleGrid_),
    activeTransform(activeTransform_), state(NO_UPDATE) { }

void TriangleUpdater::handleActiveXformChanged(int id) {
    activeTransform = id;
}

UpdateState TriangleUpdater::getUpdateState() {
    return state;
}

void TriangleUpdater::startGridDrag(WindowPoint clickPoint) {
    state = DRAGGING_GRID;
    gridDragInverse = triangleGrid->getAffineMatrix();
    gridDragInverse.Invert();
    startPoint = gridDragInverse.TransformPoint(clickPoint);
    startCenter = triangleGrid->getCenter();
}

void TriangleUpdater::startTriangleDrag(WindowPoint clickPoint) {
    state = DRAGGING_TRIANGLE;
    startPoint = triangleGrid->transformToGrid(clickPoint);
    auto coefs = flame->xforms.get(activeTransform)->coefs.value();
    startO = GridPoint(coefs.ox, coefs.oy);
}

void TriangleUpdater::startXDrag(WindowPoint clickPoint) {
    state = DRAGGING_X;
    startPoint = triangleGrid->transformToGrid(clickPoint);
    auto coefs = flame->xforms.get(activeTransform)->coefs.value();
    startX = GridPoint(coefs.xx, coefs.xy);
}

void TriangleUpdater::startYDrag(WindowPoint clickPoint) {
    state = DRAGGING_Y;
    startPoint = triangleGrid->transformToGrid(clickPoint);
    auto coefs = flame->xforms.get(activeTransform)->coefs.value();
    startY = GridPoint(coefs.yx, coefs.yy);
}

void TriangleUpdater::startTriangleRotation(WindowPoint clickPoint) {
    state = ROTATING_TRIANGLE;
    startPoint = triangleGrid->transformToGrid(clickPoint);
    auto coefs = flame->xforms.get(activeTransform)->coefs.value();
    startX = GridPoint(coefs.xx, coefs.xy);
    startY = GridPoint(coefs.yx, coefs.yy);
}

void TriangleUpdater::startTriangleScaling(WindowPoint clickPoint) {
    state = SCALING_TRIANGLE;
}

void TriangleUpdater::setUpdatePoint(WindowPoint mousePoint) {
    switch(state) {
        case DRAGGING_GRID: {
            auto updatePoint = gridDragInverse.TransformPoint(mousePoint);
            triangleGrid->updateCenter(startCenter + startPoint - updatePoint);
            break;
        }
        case DRAGGING_TRIANGLE: {
            auto updatePoint = triangleGrid->transformToGrid(mousePoint);
            auto newO = startO + updatePoint - startPoint;
            auto coefs = flame->xforms.get(activeTransform)->coefs.get();
            coefs->ox = newO.m_x;
            coefs->oy = newO.m_y;
            break;
        }
        case DRAGGING_X: {
            auto updatePoint = triangleGrid->transformToGrid(mousePoint);
            auto newX = startX + updatePoint - startPoint;
            auto coefs = flame->xforms.get(activeTransform)->coefs.get();
            coefs->xx = newX.m_x;
            coefs->xy = newX.m_y;
            break;
        }
        case DRAGGING_Y: {
            auto updatePoint = triangleGrid->transformToGrid(mousePoint);
            auto newY = startY + updatePoint - startPoint;
            auto coefs = flame->xforms.get(activeTransform)->coefs.get();
            coefs->yx = newY.m_x;
            coefs->yy = newY.m_y;
            break;
        }
        case ROTATING_TRIANGLE: {
            auto updatePoint = triangleGrid->transformToGrid(mousePoint);
            auto coefs = flame->xforms.get(activeTransform)->coefs.get();
            GridPoint origin(coefs->ox, coefs->oy);
            double rot = (updatePoint-origin).GetVectorAngle()-(startPoint-origin).GetVectorAngle();
            auto newX = startX;
            newX.SetVectorAngle(startX.GetVectorAngle()+rot);
            auto newY = startY;
            newY.SetVectorAngle(startY.GetVectorAngle()+rot);
            coefs->xx = newX.m_x;
            coefs->xy = newX.m_y;
            coefs->yx = newY.m_x;
            coefs->yy = newY.m_y;
            break;
        }
        default: break;
    }
}

void TriangleUpdater::finishUpdate() {
    state = NO_UPDATE;
}


}