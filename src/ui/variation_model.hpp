#pragma once

#include "content.hpp"
#include "view_model.hpp"
#include "../core/flame.hpp"
#include <boost/signals2.hpp>

namespace ui {

struct VariationDataParams {
    core::Variation::VariationID id;
    core::VariationData data;
};

class VariationModel: public ViewModel {
public:
    VariationModel(wxDataViewListCtrl* variationCtrl, wxTextCtrl* variationAddCtrl);
    void handleVariationAdd();
    void handleFlameContent(std::optional<FlameContent> flameContent);
    void handleNoContent();
    void handleContent(VariationContent content);
    boost::signals2::signal<void (core::Variation::VariationID)> variationAdded;
    boost::signals2::signal<void (VariationDataParams)> variationData;
private:
    void getValues(std::vector<wxVector<wxVariant>>& data) const override;
    void setValue(const wxVariant& value, int row, int col) override;
    void afterUpdate(int selectedRow) override;

    std::optional<VariationContent> content;
    wxTextCtrl* variationAddCtrl;
};

}