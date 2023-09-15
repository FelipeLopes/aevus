#include "flame_model.hpp"
#include <wx/graphics.h>
#include <wx/dcbuffer.h>

namespace ui {

FlameModel::FlameModel(wxScrolledWindow* flameWindow_): flameWindow(flameWindow_),
    zoomLevel(0), zoomFactor(1.1)
{
    flameWindow->SetBackgroundStyle(wxBG_STYLE_PAINT);
}

void FlameModel::handlePaint() {
    wxAutoBufferedPaintDC dc(flameWindow);
    dc.SetBackground(*wxGREY_BRUSH);
    dc.Clear();
    flameWindow->DoPrepareDC(dc);
    wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
    if (gc) {
        drawFlameBitmap(gc);
        delete gc;
    }
}

void FlameModel::handleMouseWheel(wxMouseEvent &event) {
    if (!flameBitmap.IsOk()) {
        return;
    }
    auto clientSize = flameWindow->GetClientSize();
    wxPoint clientCenter(clientSize.GetWidth() / 2, clientSize.GetHeight() / 2);
    auto prevCenter = flameWindow->CalcUnscrolledPosition(clientCenter);
    wxPoint nextCenter;
    if (event.GetWheelRotation() > 0 && zoomLevel < MAX_ZOOM_LEVEL) {
        zoomLevel++;
        nextCenter = prevCenter * zoomFactor;
    } else if (event.GetWheelRotation() < 0 && zoomLevel > -MAX_ZOOM_LEVEL) {
        zoomLevel--;
        nextCenter = prevCenter * (1.0 / zoomFactor);
    } else {
        return;
    }
    wxPoint pixelsPerUnit;
    flameWindow->GetScrollPixelsPerUnit(&pixelsPerUnit.x, &pixelsPerUnit.y);
    auto diff = nextCenter - prevCenter;
    auto newX = flameWindow->GetViewStart().x + diff.x / pixelsPerUnit.x;
    auto newY = flameWindow->GetViewStart().y + diff.y / pixelsPerUnit.y;
    flameWindow->Scroll(newX, newY);
    flameWindow->SetVirtualSize(scaledImageSize());
    flameWindow->Refresh();
}

void FlameModel::setBitmap(const wxBitmap &bitmap) {
    bitmapLock.lock();
    flameBitmap = bitmap;
    bitmapLock.unlock();
    flameWindow->SetVirtualSize(scaledImageSize());
    flameWindow->Refresh();
}

void FlameModel::drawFlameBitmap(wxGraphicsContext* gc) {
    auto drawSize = flameWindow->GetVirtualSize();
    bitmapLock.lock();
    if (flameBitmap.IsOk()) {
        auto imageSize = scaledImageSize();
        double bitmapWidth = imageSize.GetWidth();
        double bitmapHeight = imageSize.GetHeight();
        double x = (drawSize.GetWidth() - bitmapWidth) / 2;
        double y = (drawSize.GetHeight() - bitmapHeight) / 2;
        gc->DrawBitmap(flameBitmap, x, y, bitmapWidth, bitmapHeight);
    }
    bitmapLock.unlock();
}

wxSize FlameModel::scaledImageSize() {
    double zoom = pow(zoomFactor, zoomLevel);
    auto bmpSize = flameBitmap.GetSize();
    return wxSize(bmpSize.GetWidth()*zoom, bmpSize.GetHeight()*zoom);
}

}