#pragma once

#include <mutex>
#include <wx/graphics.h>
#include <wx/scrolwin.h>

namespace ui {

class FlameModel {
public:
    FlameModel(wxScrolledWindow* flameWindow, std::stringstream& flameStream);
    void update();
    void handlePaint();
    void handleMouseWheel(wxMouseEvent& event);
    void setBitmap(const wxBitmap& bitmap);
private:
    void drawFlameBitmap(wxGraphicsContext* gc);
    wxSize scaledImageSize();
    std::mutex bitmapLock;
    wxScrolledWindow* flameWindow;
    std::stringstream& flameStream;
    wxBitmap flameBitmap;
    int zoomLevel;
    const double zoomFactor;
    static const int MAX_ZOOM_LEVEL = 400;
};

}