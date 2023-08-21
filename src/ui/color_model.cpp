#include "color_model.hpp"
#include "view_model.hpp"
#include <wx/rawbmp.h>

using std::shared_ptr;
using std::to_string;
using std::vector;
using core::Flame;

namespace ui {

ColorModel::ColorModel(shared_ptr<Flame> flame_, wxDataViewListCtrl* colorListCtrl,
    wxStaticBitmap* paletteBitmap_): ViewModel(colorListCtrl), flame(flame_),
    paletteBitmap(paletteBitmap_), activeTransform(0) { }

void ColorModel::drawPalette() {
    auto flamePalette = flame->palette.colors.value();
    int width =  paletteBitmap->GetSize().GetWidth();
    wxBitmap palette(width, 256);
    wxNativePixelData data(palette);
    wxNativePixelData::Iterator p(data);
    for (int y=0; y<256; y++) {
        wxNativePixelData::Iterator row = p;
        for (int x=0; x<width; x++) {
            core::Color color = flamePalette.colorAt(255-y);
            p.Red() = color.r;
            p.Green() = color.g;
            p.Blue() = color.b;
            ++p;
        }
        p = row;
        p.OffsetY(data, 1);
    }
    paletteBitmap->SetBitmap(palette);
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
    update();
}

}