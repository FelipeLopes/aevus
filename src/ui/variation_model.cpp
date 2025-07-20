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

void VariationModel::handleContent(VariationContent content) {
    this->content = content;
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
    auto varLookup = core::VariationLookup::getInstance();
    for (auto el: content.variations) {
        wxVector<wxVariant> row;
        row.push_back(varLookup->idToName(el.first));
        row.push_back(to_string(el.second.weight));
        data.push_back(row);
    }
}

void VariationModel::setValue(const wxVariant& val, int row, int col) {
    if (col == 0) {
        update();
        return;
    }
    auto it = content.variations.begin();
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
    if (!content.flameLoaded) {
        variationAddCtrl->ChangeValue("");
        variationAddCtrl->Disable();
    } else {
        variationAddCtrl->Enable();
    }
}

}