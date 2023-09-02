#include "weights_model.hpp"
#include "selection_view_model.hpp"
#include <string>

using core::Flame;
using std::shared_ptr;
using std::string;
using std::to_string;
using std::vector;

namespace ui {

WeightsModel::WeightsModel(shared_ptr<Flame> flame_, wxDataViewListCtrl* weightsListCtrl):
    SelectionViewModel(weightsListCtrl), flame(flame_), blockSelectionEvents(false) { }

void WeightsModel::handleSelectionEvent(wxDataViewEvent& event) {
    if (!updating() && !blockSelectionEvents) {
        xformSelected(getSelectedRow());
    }
}

void WeightsModel::handleActiveXformChanged(int id) {
    blockSelectionEvents = true;
    selectRow(id);
    blockSelectionEvents = false;
}

void WeightsModel::getValues(vector<wxVector<wxVariant>>& data) const {
    int sz = flame->xforms.size();
    for (int i=0; i<sz; i++) {
        wxVector<wxVariant> row;
        row.push_back(to_string(i+1));
        row.push_back(to_string(flame->xforms.get(i)->weight.value()));
        data.push_back(row);
    }
}

void WeightsModel::setValue(const wxVariant& val, int row, int col) {
    if (col == 0) {
        update();
        return;
    }
    double oldValue = flame->xforms.get(row)->weight.value();
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
    flame->xforms.get(row)->weight.setValue(newValue);
    weightsChanged();
}

}