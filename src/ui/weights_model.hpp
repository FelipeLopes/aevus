#pragma once

#include "selection_view_model.hpp"

namespace ui {

class WeightsModel: public SelectionViewModel {
public:
    WeightsModel(wxDataViewListCtrl* weightsListCtrl);
private:
    int getCount() const override;
    wxVariant getValue(int row, int col) const override;
    void setValue(const wxVariant& value, int row, int col) override;
};

}