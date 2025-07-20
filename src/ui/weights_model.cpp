#include "weights_model.hpp"
#include "content.hpp"
#include "selection_view_model.hpp"
#include <string>

using std::string;
using std::to_string;
using std::vector;

namespace ui {

WeightsModel::WeightsModel(wxDataViewListCtrl* weightsListCtrl, wxBitmapButton* addXformButton_,
    wxBitmapButton* removeXformButton_): SelectionViewModel(weightsListCtrl),
    addXformButton(addXformButton_), removeXformButton(removeXformButton_),
    blockSelectionEvents(false)
{
    addXformButton->Disable();
    removeXformButton->Disable();
}

void WeightsModel::handleSelectionEvent(wxDataViewEvent& event) {
    if (!updating() && !blockSelectionEvents) {
        content.activeId = getSelectedRow();
        xformSelected(content.activeId);
    }
}

void WeightsModel::handleContent(WeightsContent content) {
    //printf("handle content begin\n");
    this->content = content;
    //printf("before update\n");
    update();
    /*if (content.activeId != -1) {
        blockSelectionEvents = true;
        printf("before select\n");
        selectRow(content.activeId);
        blockSelectionEvents = false;
    }
    printf("handle content end\n");*/
}

void WeightsModel::handleAddXform() {
    xformAdded();
}

void WeightsModel::handleRemoveXform() {
    xformRemoved();
}

void WeightsModel::getValues(vector<wxVector<wxVariant>>& data) const {
    if (!content.flameLoaded) {
        return;
    }
    int sz = content.weights.size();
    for (int i=0; i<sz; i++) {
        wxVector<wxVariant> row;
        row.push_back(to_string(i+1));
        row.push_back(to_string(content.weights[i]));
        data.push_back(row);
    }
}

void WeightsModel::setValue(const wxVariant& val, int row, int col) {
    if (col == 0) {
        update();
        return;
    }
    double oldValue = content.weights[row];
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
    content.weights[row] = newValue;
    weightsChanged(content);
}

void WeightsModel::afterUpdate(int selectedRow) {
    if (content.activeId != -1) {
        SelectionViewModel::afterUpdate(content.activeId);
    }
    if (!content.flameLoaded) {
        addXformButton->Disable();
    } else {
        addXformButton->Enable();
    }
    if (content.activeId == -1) {
        removeXformButton->Disable();
    } else {
        removeXformButton->Enable();
    }
}

}