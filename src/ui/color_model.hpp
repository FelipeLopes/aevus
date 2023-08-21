#pragma once

#include "view_model.hpp"
#include "../core/flame.hpp"
#include <boost/signals2.hpp>
#include <wx/statbmp.h>

namespace ui {

class ColorModel: public ViewModel {
public:
    ColorModel(std::shared_ptr<core::Flame> flame, wxDataViewListCtrl* colorListCtrl,
        wxStaticBitmap* paletteBitmap);
    void drawPalette();
    boost::signals2::signal<void ()> colorChanged;
private:
    void getValues(std::vector<wxVector<wxVariant>>& data) const override;
    void setValue(const wxVariant& value, int row, int col) override;

    std::shared_ptr<core::Flame> flame;
    wxStaticBitmap* paletteBitmap;
    int activeTransform;
};

}