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
    auto drawSize = flameWindow->GetVirtualSize();
    if (gc) {
        if (flameBitmap.IsOk()) {
            auto imageSize = scaledImageSize();
            double bitmapWidth = imageSize.GetWidth();
            double bitmapHeight = imageSize.GetHeight();
            double x = (drawSize.GetWidth() - bitmapWidth) / 2;
            double y = (drawSize.GetHeight() - bitmapHeight) / 2;
            gc->DrawBitmap(flameBitmap, x, y, bitmapWidth, bitmapHeight);
        }
        delete gc;
    }
}

void FlameModel::handleMouseWheel(wxMouseEvent &event) {
    if (!flameBitmap.IsOk()) {
        return;
    }
    if (event.GetWheelRotation() > 0) {
        zoomIn();
    } else if (event.GetWheelRotation() < 0) {
        zoomOut();
    }
    flameWindow->Refresh();
}

void FlameModel::setBitmap(const wxBitmap &bitmap) {
    flameBitmap = bitmap;
    flameWindow->SetVirtualSize(scaledImageSize());
    flameWindow->Refresh();
}

void FlameModel::zoomIn() {
    if (zoomLevel < MAX_ZOOM_LEVEL) {
        zoomLevel++;
    }
    flameWindow->SetVirtualSize(scaledImageSize());
}

void FlameModel::zoomOut() {
    if (zoomLevel > -MAX_ZOOM_LEVEL) {
        zoomLevel--;
    }
    flameWindow->SetVirtualSize(scaledImageSize());
}

wxSize FlameModel::scaledImageSize() {
    double zoom = pow(zoomFactor, zoomLevel);
    auto bmpSize = flameBitmap.GetSize();
    return wxSize(bmpSize.GetWidth()*zoom, bmpSize.GetHeight()*zoom);
}

}