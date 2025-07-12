#include "weights_model.hpp"
#include "selection_view_model.hpp"
#include <string>

using std::string;
using std::to_string;
using std::vector;

namespace ui {

WeightsModel::WeightsModel(wxDataViewListCtrl* weightsListCtrl, wxBitmapButton* addXformButton_,
    wxBitmapButton* removeXformButton_): SelectionViewModel(weightsListCtrl),
    addXformButton(addXformButton_), removeXformButton(removeXformButton_),
    activeTransform(-1), blockSelectionEvents(false)
{
    addXformButton->Disable();
    removeXformButton->Disable();
}

void WeightsModel::handleSelectionEvent(wxDataViewEvent& event) {
    if (!updating() && !blockSelectionEvents) {
        activeTransform = getSelectedRow();
        xformSelected(activeTransform);
    }
}

void WeightsModel::handleActiveXformChanged(int id) {
    activeTransform = id;
    if (id != -1) {
        blockSelectionEvents = true;
        selectRow(id);
        blockSelectionEvents = false;
    }
    update();
}

void WeightsModel::handleAddXform() {
    int id = flame->xforms.size();
    flame->xforms.append(std::make_shared<core::XForm>());
    weightsChanged();
    xformSelected(id);
}

void WeightsModel::handleRemoveXform() {
    int id = getSelectedRow();
    int sz = flame->xforms.size();
    flame->xforms.remove(id);
    if (id == sz - 1) {
        id--;
    }
    xformSelected(id);
    weightsChanged();
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

void WeightsModel::afterUpdate(int selectedRow) {
    SelectionViewModel::afterUpdate(selectedRow);
    if (flame == NULL) {
        addXformButton->Disable();
    } else {
        addXformButton->Enable();
    }
    if (activeTransform == -1) {
        removeXformButton->Disable();
    } else {
        removeXformButton->Enable();
    }
}

}