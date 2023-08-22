#include "color_model.hpp"
#include "view_model.hpp"
#include <algorithm>
#include <wx/rawbmp.h>
#include <wx/graphics.h>
#include <wx/dcbuffer.h>

using std::shared_ptr;
using std::to_string;
using std::vector;
using core::Color;
using core::Flame;

namespace ui {

ColorModel::ColorModel(shared_ptr<Flame> flame_, wxDataViewListCtrl* colorListCtrl,
    wxPanel* palettePanel_): ViewModel(colorListCtrl), flame(flame_),
    palettePanel(palettePanel_),
    paletteWidth(palettePanel->GetSize().GetWidth()),
    paletteBitmap(paletteWidth, 256),
    activeTransform(0)
{
    palettePanel->SetBackgroundStyle(wxBG_STYLE_PAINT);
}

void ColorModel::setupPalette() {
    auto flamePalette = flame->palette.colors.value();
    wxNativePixelData data(paletteBitmap);
    wxNativePixelData::Iterator p(data);
    for (int y=0; y<256; y++) {
        wxNativePixelData::Iterator row = p;
        for (int x=0; x<paletteWidth; x++) {
            Color color = flamePalette.colorAt(255-y);
            p.Red() = color.r;
            p.Green() = color.g;
            p.Blue() = color.b;
            ++p;
        }
        p = row;
        p.OffsetY(data, 1);
    }
}

void ColorModel::handleActiveXformChanged(int id) {
    activeTransform = id;
    update();
}

void ColorModel::handlePaint() {
    wxAutoBufferedPaintDC dc(palettePanel);
    wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
    if (gc) {
        gc->DrawBitmap(paletteBitmap, 0, 0, paletteWidth, 256);
        delete gc;
    }
}

void ColorModel::getValues(vector<wxVector<wxVariant>>& data) const {
    wxVector<wxVariant> firstRow;
    firstRow.push_back("position");
    firstRow.push_back(to_string(flame->xforms.get(activeTransform)->color.getValue()));
    data.push_back(firstRow);
    wxVector<wxVariant> secondRow;
    secondRow.push_back("speed");
    secondRow.push_back(
        to_string(flame->xforms.get(activeTransform)->colorSpeed.value().colorSpeed));
    data.push_back(secondRow);
}

void ColorModel::setValue(const wxVariant& val, int row, int col) {
    printf("setValue called\n");
    update();
}

}