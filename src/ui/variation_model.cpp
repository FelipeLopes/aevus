#include "variation_model.hpp"
#include "variation_text_completer.hpp"
#include <stdexcept>

using std::string;
using std::to_string;
using std::vector;

namespace ui {

VariationModel::VariationModel(wxDataViewListCtrl* variationCtrl,
    wxTextCtrl* variationAddCtrl_): ViewModel(variationCtrl),
    activeTransform(-1), variationAddCtrl(variationAddCtrl_)
{
    variationAddCtrl->AutoComplete(new VariationTextCompleter);
}

void VariationModel::handleActiveXformChanged(int id) {
    activeTransform = id;
    update();
}

void VariationModel::handleVariationAdd() {
    string text = variationAddCtrl->GetValue().ToStdString();
    auto varLookup = core::VariationLookup::getInstance();
    try {
        auto id = varLookup->nameToId(text);
        auto varMap = &flame->xforms.get(activeTransform)->variationMap.get()->variations;
        if (varMap->find(id) != varMap->end()) {
            variationAddCtrl->ChangeValue("");
            return;
        }
        (*varMap)[id] = core::VariationData(1.0, {});
        variationAddCtrl->ChangeValue("");
        variationDataChanged();
    } catch (std::invalid_argument& e) {
        return;
    }
}

void VariationModel::getValues(vector<wxVector<wxVariant>>& data) const {
    if (flame == NULL || activeTransform == -1) {
        return;
    }
    auto varMap = flame->xforms.get(activeTransform)->variationMap.get()->variations;
    auto varLookup = core::VariationLookup::getInstance();
    for (auto el: varMap) {
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
    auto varMap = &flame->xforms.get(activeTransform)->variationMap.get()->variations;
    auto it = varMap->begin();
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
    if (newValue == 0.0) {
        varMap->erase(it->first);
    } else {
        (*varMap)[it->first].weight = newValue;
    }
    variationDataChanged();
}

void VariationModel::afterUpdate(int selectedRow) {
    if (activeTransform == -1) {
        variationAddCtrl->ChangeValue("");
        variationAddCtrl->Disable();
    } else {
        variationAddCtrl->Enable();
    }
}

}