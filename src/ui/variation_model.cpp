#include "variation_model.hpp"

using std::shared_ptr;
using std::to_string;
using std::vector;
using core::Flame;

namespace ui {

VariationModel::VariationModel(shared_ptr<Flame> flame_, wxDataViewListCtrl* variationCtrl):
    ViewModel(variationCtrl), flame(flame_) { }

void VariationModel::handleActiveXformChanged(int id) {
    activeTransform = id;
    update();
}

void VariationModel::getValues(vector<wxVector<wxVariant>>& data) const {
    auto varMap = flame->xforms.get(activeTransform)->variationMap.get()->variations;
    auto varLookup = core::VariationLookup::getInstance();
    for (auto el: varMap) {
        wxVector<wxVariant> row;
        row.push_back(varLookup->idToName(el.first));
        row.push_back(to_string(el.second));
        data.push_back(row);
    }
}

}