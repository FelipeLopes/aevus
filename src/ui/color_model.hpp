#pragma once

#include "view_model.hpp"
#include "../core/flame.hpp"
#include <cfloat>
#include <boost/signals2.hpp>
#include <wx/panel.h>

namespace ui {

class ColorModel: public ViewModel {
public:
    ColorModel(std::shared_ptr<core::Flame> flame, wxDataViewListCtrl* colorListCtrl,
        wxPanel* palettePanel);
    void handleActiveXformChanged(int id);
    void handlePaint();
    void handleMouseUp(wxMouseEvent& event);
    void handleMouseDown(wxMouseEvent& event);
    void handleMouseMove(wxMouseEvent& event);
    void setupPalette();
    boost::signals2::signal<void ()> colorChanged;
private:
    void getValues(std::vector<wxVector<wxVariant>>& data) const override;
    void setValue(const wxVariant& value, int row, int col) override;
    void afterUpdate(int selectedRow) override;

    std::shared_ptr<core::Flame> flame;
    wxPanel* palettePanel;
    int paletteWidth;
    wxBitmap paletteBitmap;
    wxBitmap blackLineBitmap;
    wxBitmap whiteLineBitmap;
    int activeTransform;
    bool dragging;

    constexpr static const float BUCKET_FACTOR = (1.0f-FLT_EPSILON);
};

}