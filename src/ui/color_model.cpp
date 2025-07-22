#include "color_model.hpp"
#include "content.hpp"
#include "view_model.hpp"
#include <algorithm>
#include <wx/rawbmp.h>
#include <wx/graphics.h>
#include <wx/dcbuffer.h>

using std::string;
using std::to_string;
using std::vector;
using core::Color;

namespace ui {

ColorModel::ColorModel(wxDataViewListCtrl* colorListCtrl, wxPanel* palettePanel_):
    ViewModel(colorListCtrl),
    palettePanel(palettePanel_),
    paletteWidth(palettePanel->GetSize().GetWidth()),
    paletteBitmap(paletteWidth, 256),
    blackLineBitmap(paletteWidth, 1),
    whiteLineBitmap(paletteWidth, 1),
    dragging(false)
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
    setupPalette();
}

void ColorModel::setupPalette() {
    if (flame == NULL) {
        return;
    }
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

void ColorModel::handleContent(ColorContent content) {
    this->content = content;
    update();
}

void ColorModel::handlePaint() {
    wxAutoBufferedPaintDC dc(palettePanel);
    dc.Clear();
    wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
    if (gc) {
        gc->DrawBitmap(paletteBitmap, 0, 0, paletteWidth, 256);
        if (content.flameLoaded) {
            /*float colorVal = content.color;
            colorVal = std::clamp(colorVal, 0.0f, BUCKET_FACTOR);
            int palettePos = (int)(colorVal*256);
            Color c = flame->palette.colors.value().colorAt(palettePos);
            int highlightLine = 255 - palettePos;
            if ((c.r*0.299 + c.g*0.587 + c.b*0.114) > 149) {
                gc->DrawBitmap(blackLineBitmap, 0, highlightLine, paletteWidth, 1);
            } else {
                gc->DrawBitmap(whiteLineBitmap, 0, highlightLine, paletteWidth, 1);
            }*/
        }
        delete gc;
    }
}

void ColorModel::afterUpdate(int selectedRow) {
    palettePanel->Refresh();
}

void ColorModel::getValues(vector<wxVector<wxVariant>>& data) const {
    if (!content.flameLoaded) {
        return;
    }
    wxVector<wxVariant> firstRow;
    firstRow.push_back("position");
    firstRow.push_back(to_string(content.color));
    data.push_back(firstRow);
    wxVector<wxVariant> secondRow;
    secondRow.push_back("speed");
    secondRow.push_back(to_string(content.colorSpeed));
    data.push_back(secondRow);
}

void ColorModel::setValue(const wxVariant& val, int row, int col) {
    if (col == 0) {
        update();
        return;
    }
    double oldValue = 0;
    switch (row) {
        case 0: oldValue = content.color; break;
        case 1: oldValue = content.colorSpeed; break;
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
    }/*
    switch (row) {
        case 0: flame->xforms.get(activeTransform)->color.setValue(newValue); break;
        case 1: flame->xforms.get(activeTransform)->colorSpeed.get()->colorSpeed = newValue; break;
    }
    colorChanged();*/
}

void ColorModel::handleMouseUp(wxMouseEvent& event) {
    dragging = false;
}

void ColorModel::handleMouseDown(wxMouseEvent& event) {
    if (content.flameLoaded) {
        dragging = true;
        auto cursorPos = event.GetPosition();
        setValue(1.0-cursorPos.y/255.0, 0, 1);
    }
}

void ColorModel::handleMouseMove(wxMouseEvent& event) {
    if (dragging) {
        auto cursorPos = event.GetPosition();
        setValue(1.0-cursorPos.y/255.0, 0, 1);
    }
}

}