#pragma once

#include "view_model.hpp"

namespace ui {

class VariationModel: public ViewModel {
public:
    VariationModel(wxDataViewListCtrl* variationCtrl);
private:
    int getCount() const override;
    wxVariant getValue(int row, int col) const override;
};

}