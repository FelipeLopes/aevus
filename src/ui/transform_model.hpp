#pragma once

#include "view_model.hpp"
#include <wx/dataview.h>

namespace ui {

class TransformModel: public ViewModel {
public:
    TransformModel(wxDataViewListCtrl* transformCtrl);
private:
    int getCount() const override;
    wxVariant getValue(int row, int col) const override;
    void setValue(const wxVariant& value, int row, int col) override;
};

}