#include "triangle_updater.hpp"
#include "content.hpp"
#include "triangle_types.hpp"

namespace ui {

TriangleUpdater::TriangleUpdater(TriangleGrid* triangleGrid_):
    triangleGrid(triangleGrid_), state(NO_UPDATE) { }

void TriangleUpdater::handleContent(std::optional<XFormTriangleContent> content) {
    this->content = content;
}

void TriangleUpdater::updateCoefsContent(int pos, CoefsContent coefsContent) {
    if (!content.has_value()) {
        return;
    }
    content->activeId = pos;
    content->coefs[content->activeId] = coefsContent;
}

UpdateState TriangleUpdater::getUpdateState() {
    return state;
}

void TriangleUpdater::setUpdateStartData(WindowPoint clickPoint) {
    if (!content.has_value()) {
        return;
    }
    startPoint = triangleGrid->transformToGrid(clickPoint);
    auto coefs = content->coefs[content->activeId];
    startO = GridPoint(coefs.ox, coefs.oy);
    startX = GridPoint(coefs.xx, coefs.xy);
    startY = GridPoint(coefs.yx, coefs.yy);
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
    setUpdateStartData(clickPoint);
}

void TriangleUpdater::startXDrag(WindowPoint clickPoint) {
    state = DRAGGING_X;
    setUpdateStartData(clickPoint);
}

void TriangleUpdater::startYDrag(WindowPoint clickPoint) {
    state = DRAGGING_Y;
    setUpdateStartData(clickPoint);
}

void TriangleUpdater::startTriangleRotation(WindowPoint clickPoint) {
    state = ROTATING_TRIANGLE;
    setUpdateStartData(clickPoint);
}

void TriangleUpdater::startTriangleScaling(WindowPoint clickPoint) {
    state = SCALING_TRIANGLE;
    setUpdateStartData(clickPoint);
}

void TriangleUpdater::updateCenterForPoint(WindowPoint mousePoint) {
    auto updatePoint = gridDragInverse.TransformPoint(mousePoint);
    triangleGrid->updateCenter(startCenter + startPoint - updatePoint);
}

CoefsContent TriangleUpdater::getCoefsForPoint(WindowPoint mousePoint) {
    CoefsContent coefs;
    coefs.ox = startO.m_x;
    coefs.oy = startO.m_y;
    coefs.xx = startX.m_x;
    coefs.xy = startX.m_y;
    coefs.yx = startY.m_x;
    coefs.yy = startY.m_y;
    switch(state) {
        case DRAGGING_TRIANGLE: {
            auto updatePoint = triangleGrid->transformToGrid(mousePoint);
            auto newO = startO + updatePoint - startPoint;
            coefs.ox = newO.m_x;
            coefs.oy = newO.m_y;
            break;
        }
        case DRAGGING_X: {
            auto updatePoint = triangleGrid->transformToGrid(mousePoint);
            auto newX = startX + updatePoint - startPoint;
            coefs.xx = newX.m_x;
            coefs.xy = newX.m_y;
            break;
        }
        case DRAGGING_Y: {
            auto updatePoint = triangleGrid->transformToGrid(mousePoint);
            auto newY = startY + updatePoint - startPoint;
            coefs.yx = newY.m_x;
            coefs.yy = newY.m_y;
            break;
        }
        case ROTATING_TRIANGLE: {
            auto updatePoint = triangleGrid->transformToGrid(mousePoint);
            GridPoint origin(coefs.ox, coefs.oy);
            double rot = (updatePoint-origin).GetVectorAngle()-(startPoint-origin).GetVectorAngle();
            auto newX = startX;
            newX.SetVectorAngle(startX.GetVectorAngle()+rot);
            auto newY = startY;
            newY.SetVectorAngle(startY.GetVectorAngle()+rot);
            coefs.xx = newX.m_x;
            coefs.xy = newX.m_y;
            coefs.yx = newY.m_x;
            coefs.yy = newY.m_y;
            break;
        }
        case SCALING_TRIANGLE: {
            auto updatePoint = triangleGrid->transformToGrid(mousePoint);
            GridPoint origin(coefs.ox, coefs.oy);
            double sc = distancePointLine(updatePoint, origin, origin+startX-startY)/
                distancePointLine(startPoint, origin, origin+startX-startY);
            auto newX = startX;
            newX.SetVectorLength(startX.GetVectorLength()*sc);
            auto newY = startY;
            newY.SetVectorLength(startY.GetVectorLength()*sc);
            coefs.xx = newX.m_x;
            coefs.xy = newX.m_y;
            coefs.yx = newY.m_x;
            coefs.yy = newY.m_y;
            break;
        }
        default: break;
    }
    return coefs;
}

void TriangleUpdater::finishUpdate() {
    state = NO_UPDATE;
}

double TriangleUpdater::distancePointLine(GridPoint p, GridPoint s1, GridPoint s2) {
    double d = s1.GetDistance(s2);
    if (d == 0) {
        return p.GetDistance(s1);
    } else {
        return (s1-p).GetCrossProduct(s2-p)/d;
    }
}


}