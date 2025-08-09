#include "variation_model.hpp"
#include "variation_text_completer.hpp"
#include <stdexcept>

using std::string;
using std::to_string;
using std::vector;

namespace ui {

VariationModel::VariationModel(wxDataViewListCtrl* variationCtrl,
    wxTextCtrl* variationAddCtrl_): ViewModel(variationCtrl),
    variationAddCtrl(variationAddCtrl_)
{
    variationAddCtrl->AutoComplete(new VariationTextCompleter);
}

void VariationModel::handleFlameContent(std::optional<FlameContent> flameContent_) {
    if (!flameContent_.has_value()) {
        content = std::nullopt;
        update();
        return;
    }
    auto flameContent = flameContent_.value();
    auto sz = flameContent.xforms.size();
    if (sz > 0) {
        content = flameContent.xforms[0].variations;
    } else {
        content = {};
    }
    update();
}

void VariationModel::handleActiveXformContent(ActiveXFormContent xformContent) {
    content = xformContent.xform.variations;
    update();
}

void VariationModel::handleVariationAdd() {
    string text = variationAddCtrl->GetValue().ToStdString();
    auto varLookup = core::VariationLookup::getInstance();
    try {
        auto id = varLookup->nameToId(text);
        variationAddCtrl->ChangeValue("");
        variationAdded(id);
    } catch (std::invalid_argument& e) {
        return;
    }
}

void VariationModel::getValues(vector<wxVector<wxVariant>>& data) const {
    if (!content.has_value()) {
        return;
    }
    auto varLookup = core::VariationLookup::getInstance();
    for (auto el: content.value()) {
        wxVector<wxVariant> row;
        row.push_back(varLookup->idToName(el.first));
        row.push_back(to_string(el.second.weight));
        data.push_back(row);
    }
}

void VariationModel::setValue(const wxVariant& val, int row, int col) {
    if (!content.has_value() || col == 0) {
        update();
        return;
    }
    auto it = content->begin();
    for (int i=0; i<row; i++) {
        ++it;
    }
    double oldValue = it->second.weight;
    string text = val.GetString().ToStdString();
    double newValue = 0;
    try {
        newValue = std::stod(text);
    } catch (std::invalid_argument& e) {
        update();
        return;
    }
    if (newValue == oldValue) {
        update();
        return;
    }
    VariationDataParams varParams;
    varParams.id = it->first;
    varParams.data = core::VariationData(newValue, {});
    variationData(varParams);
}

void VariationModel::afterUpdate(int selectedRow) {
    if (!content.has_value()) {
        variationAddCtrl->ChangeValue("");
        variationAddCtrl->Disable();
    } else {
        variationAddCtrl->Enable();
    }
}

}