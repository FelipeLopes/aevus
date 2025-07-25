#pragma once

#include "content.hpp"
#include "view_model.hpp"
#include "../core/flame.hpp"
#include <cfloat>
#include <boost/signals2.hpp>
#include <wx/panel.h>

namespace ui {

class ColorModel: public ViewModel {
public:
    ColorModel(wxDataViewListCtrl* colorListCtrl, wxPanel* palettePanel);
    void handleContent(ColorContent content);
    void handleFlameContent(FlameContent flameContent);
    void handlePaint();
    void handleMouseUp(wxMouseEvent& event);
    void handleMouseDown(wxMouseEvent& event);
    void handleMouseMove(wxMouseEvent& event);
    boost::signals2::signal<void (ColorContent)> colorContent;
private:
    void setupPalette();
    void getValues(std::vector<wxVector<wxVariant>>& data) const override;
    void setValue(const wxVariant& value, int row, int col) override;
    void afterUpdate(int selectedRow) override;

    wxPanel* palettePanel;
    int paletteWidth;
    wxBitmap paletteBitmap;
    wxBitmap blackLineBitmap;
    wxBitmap whiteLineBitmap;
    ColorContent content;
    bool dragging;

    constexpr static const float BUCKET_FACTOR = (1.0f-FLT_EPSILON);
};

}