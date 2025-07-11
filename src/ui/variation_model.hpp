#pragma once

#include "view_model.hpp"
#include "../core/flame.hpp"
#include <boost/signals2.hpp>

namespace ui {

class VariationModel: public ViewModel {
public:
    VariationModel(wxDataViewListCtrl* variationCtrl, wxTextCtrl* variationAddCtrl);
    void handleActiveXformChanged(int id);
    void handleVariationAdd();
    boost::signals2::signal<void ()> variationDataChanged;
private:
    void getValues(std::vector<wxVector<wxVariant>>& data) const override;
    void setValue(const wxVariant& value, int row, int col) override;
    void afterUpdate(int selectedRow) override;

    int activeTransform;
    wxTextCtrl* variationAddCtrl;
};

}