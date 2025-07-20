#pragma once

#include "content.hpp"
#include "view_model.hpp"
#include "../core/flame.hpp"
#include <boost/signals2.hpp>

namespace ui {

class VariationModel: public ViewModel {
public:
    VariationModel(wxDataViewListCtrl* variationCtrl, wxTextCtrl* variationAddCtrl);
    void handleVariationAdd();
    void handleNoContent();
    void handleContent(VariationContent content);
    boost::signals2::signal<void ()> variationDataChanged;
private:
    void getValues(std::vector<wxVector<wxVariant>>& data) const override;
    void setValue(const wxVariant& value, int row, int col) override;
    void afterUpdate(int selectedRow) override;

    VariationContent content;
    wxTextCtrl* variationAddCtrl;
};

}