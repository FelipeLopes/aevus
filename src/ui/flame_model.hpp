#pragma once

#include <wx/scrolwin.h>

namespace ui {

class FlameModel {
public:
    FlameModel(wxScrolledWindow* flameWindow);
    void handlePaint();
    void handleMouseWheel(wxMouseEvent& event);
    void setBitmap(const wxBitmap& bitmap);
private:
    wxSize scaledImageSize();
    wxScrolledWindow* flameWindow;
    wxBitmap flameBitmap;
    int zoomLevel;
    const double zoomFactor;
    static const int MAX_ZOOM_LEVEL = 400;
};

}