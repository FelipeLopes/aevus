#include "color_model.hpp"
#include "view_model.hpp"
#include <algorithm>
#include <wx/rawbmp.h>
#include <wx/graphics.h>
#include <wx/dcbuffer.h>

using std::shared_ptr;
using std::string;
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
    blackLineBitmap(paletteWidth, 1),
    whiteLineBitmap(paletteWidth, 1),
    activeTransform(0)
{
    palettePanel->SetBackgroundStyle(wxBG_STYLE_PAINT);
    wxNativePixelData blackData(blackLineBitmap);
    wxNativePixelData::Iterator p(blackData);
    for (int x=0; x<paletteWidth; x++) {
        p.Red() = 0;
        p.Green() = 0;
        p.Blue() = 0;
        ++p;
    }
    wxNativePixelData whiteData(whiteLineBitmap);
    wxNativePixelData::Iterator q(whiteData);
    for (int x=0; x<paletteWidth; x++) {
        q.Red() = 255;
        q.Green() = 255;
        q.Blue() = 255;
        ++q;
    }
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
    dc.Clear();
    wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
    if (gc) {
        float colorVal = flame->xforms.get(activeTransform)->color.getValue();
        colorVal = std::clamp(colorVal, 0.0f, BUCKET_FACTOR);
        int palettePos = (int)(colorVal*256);
        Color c = flame->palette.colors.value().colorAt(palettePos);
        int highlightLine = 255 - palettePos;
        gc->DrawBitmap(paletteBitmap, 0, 0, paletteWidth, 256);
        if ((c.r*0.299 + c.g*0.587 + c.b*0.114) > 149) {
            gc->DrawBitmap(blackLineBitmap, 0, highlightLine, paletteWidth, 1);
        } else {
            gc->DrawBitmap(whiteLineBitmap, 0, highlightLine, paletteWidth, 1);
        }
        delete gc;
    }
}

void ColorModel::afterUpdate(int selectedRow) {
    palettePanel->Refresh();
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
    if (col == 0) {
        update();
        return;
    }
    double oldValue = 0;
    switch (row) {
        case 0: oldValue = flame->xforms.get(activeTransform)->color.getValue(); break;
        case 1: oldValue = flame->xforms.get(activeTransform)->colorSpeed.value().colorSpeed; break;
    }
    string text = val.GetString().ToStdString();
    double newValue = 0;
    try {
        newValue = std::stod(text);
    } catch (std::invalid_argument& e) {
        update();
        return;
    }
    newValue = std::clamp(newValue, 0.0, 1.0);
    if (newValue == oldValue) {
        update();
        return;
    }
    switch (row) {
        case 0: flame->xforms.get(activeTransform)->color.setValue(newValue); break;
        case 1: flame->xforms.get(activeTransform)->colorSpeed.get()->colorSpeed = newValue; break;
    }
    colorChanged();
}

}