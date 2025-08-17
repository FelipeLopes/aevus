#pragma once

#include "../core/content.hpp"
#include "view_model.hpp"
#include "../core/flame.hpp"
#include <boost/signals2.hpp>

namespace ui {

class VariationModel: public ViewModel {
public:
    VariationModel(wxDataViewListCtrl* variationCtrl, wxTextCtrl* variationAddCtrl);
    void handleVariationAdd();
    void handleFlameContent(std::optional<core::FlameContent> flameContent);
    void handleActiveXformContent(core::ActiveXFormContent xformContent);
    boost::signals2::signal<void (core::ActiveXFormUpdateContent)> xformUpdate;
private:
    void getValues(std::vector<wxVector<wxVariant>>& data) const override;
    void setValue(const wxVariant& value, int row, int col) override;
    void afterUpdate(int selectedRow) override;

    std::optional<core::VariationContent> content;
    wxTextCtrl* variationAddCtrl;
};

}