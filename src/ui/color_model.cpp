#include "color_model.hpp"
#include "view_model.hpp"
#include <algorithm>
#include <cmath>
#include <wx/rawbmp.h>
#include <wx/graphics.h>
#include <wx/dcbuffer.h>

using core::FlameContent;
using core::XFormColorContent;
using core::ActiveXFormContent;
using core::ActiveXFormUpdateContent;
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
    wxNativePixelData data(paletteBitmap);
    wxNativePixelData::Iterator p(data);
    for (int y=0; y<256; y++) {
        wxNativePixelData::Iterator row = p;
        if (colormapContent.has_value()) {
            colormapContent->computeColormapCL(colormapCL);
        }
        for (int x=0; x<paletteWidth; x++) {
            Color color;
            if (colormapContent.has_value()) {
                core::ColorCL colorCL = colormapCL[255-y];
                color.r = lround(colorCL.r * 255.0);
                color.g = lround(colorCL.g * 255.0);
                color.b = lround(colorCL.b * 255.0);
            }
            p.Red() = color.r;
            p.Green() = color.g;
            p.Blue() = color.b;
            ++p;
        }
        p = row;
        p.OffsetY(data, 1);
    }
}

void ColorModel::handleFlameContent(std::optional<FlameContent> flameContent_) {
    if (!flameContent_.has_value()) {
        xformColorContent = std::nullopt;
        colormapContent = std::nullopt;
        setupPalette();
        update();
        return;
    }
    xformColorContent = XFormColorContent();
    auto flameContent = flameContent_.value();
    colormapContent = flameContent.colormap;
    auto sz = flameContent.xforms.size();
    if (sz > 0) {
        xformColorContent->color = flameContent.xforms[0].color;
        xformColorContent->colorSpeed = flameContent.xforms[0].colorSpeed;
    }
    setupPalette();
    update();
}

void ColorModel::handleColormapContent(core::ColormapContent colormapContent_) {
    colormapContent = colormapContent_;
    setupPalette();
    update();
}

void ColorModel::handleActiveXformContent(ActiveXFormContent xformContent) {
    if (xformContent.xform.has_value()) {
        if (!xformColorContent.has_value()) {
            xformColorContent = XFormColorContent();
        }
        xformColorContent->color = xformContent.xform->color;
        xformColorContent->colorSpeed = xformContent.xform->colorSpeed;
    } else {
        xformColorContent = std::nullopt;
    }
    update();
}

void ColorModel::handlePaint() {
    wxAutoBufferedPaintDC dc(palettePanel);
    dc.Clear();
    wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
    if (gc) {
        gc->DrawBitmap(paletteBitmap, 0, 0, paletteWidth, 256);
        if (xformColorContent.has_value()) {
            float colorVal = xformColorContent->color;
            colorVal = std::clamp(colorVal, 0.0f, BUCKET_FACTOR);
            int palettePos = (int)(colorVal*256);
            Color c;
            core::ColorCL colorCL = colormapCL[palettePos];
            c.r = lround(colorCL.r * 255.0);
            c.g = lround(colorCL.g * 255.0);
            c.b = lround(colorCL.b * 255.0);
            int highlightLine = 255 - palettePos;
            if ((c.r*0.299 + c.g*0.587 + c.b*0.114) > 149) {
                gc->DrawBitmap(blackLineBitmap, 0, highlightLine, paletteWidth, 1);
            } else {
                gc->DrawBitmap(whiteLineBitmap, 0, highlightLine, paletteWidth, 1);
            }
        }
        delete gc;
    }
}

void ColorModel::afterUpdate(int selectedRow) {
    palettePanel->Refresh();
}

void ColorModel::getValues(vector<wxVector<wxVariant>>& data) const {
    if (!xformColorContent.has_value()) {
        return;
    }
    wxVector<wxVariant> firstRow;
    firstRow.push_back("position");
    firstRow.push_back(to_string(xformColorContent->color));
    data.push_back(firstRow);
    wxVector<wxVariant> secondRow;
    secondRow.push_back("speed");
    secondRow.push_back(to_string(xformColorContent->colorSpeed));
    data.push_back(secondRow);
}

void ColorModel::setValue(const wxVariant& val, int row, int col) {
    if (col == 0) {
        update();
        return;
    }
    double oldValue = 0;
    switch (row) {
        case 0: oldValue = xformColorContent->color; break;
        case 1: oldValue = xformColorContent->colorSpeed; break;
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
    ActiveXFormUpdateContent updateContent;
    switch (row) {
        case 0:
            xformColorContent->color = newValue;
            updateContent.color = newValue;
            break;
        case 1:
            xformColorContent->colorSpeed = newValue;
            updateContent.colorSpeed = newValue;
            break;
    }
    xformUpdate(updateContent);
    update();
}

void ColorModel::handleMouseUp() {
    if (xformColorContent.has_value() && dragging) {
        dragging = false;
        stopXFormExplore();
    }
}

void ColorModel::handleMouseDown(wxMouseEvent& event) {
    if (xformColorContent.has_value()) {
        startXFormExplore();
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