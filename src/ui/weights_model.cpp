#include "weights_model.hpp"
#include "selection_view_model.hpp"
#include <string>

using core::FlameContent;
using core::WeightsContent;
using core::ActiveXFormContent;
using core::ActiveXFormUpdateContent;
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
    if (!updating() && !blockSelectionEvents && content.has_value()) {
        content->activeId = getSelectedRow();
        xformSelected(content->activeId);
    }
}

void WeightsModel::handleFlameContent(std::optional<FlameContent> flameContent_) {
    if (!flameContent_.has_value()) {
        content = std::nullopt;
        update();
        return;
    }
    content = WeightsContent();
    auto flameContent = flameContent_.value();
    auto sz = flameContent.xforms.size();
    content->activeId = (sz == 0) ? -1 : 0;
    content->weights.resize(sz);
    for (int i=0; i<sz; i++) {
        content->weights[i] = flameContent.xforms[i].weight;
    }
    update();
}

void WeightsModel::handleActiveXformContent(ActiveXFormContent xformContent) {
    if (!content.has_value()) {
        return;
    }
    content->activeId = xformContent.id;
    auto it = std::next(content->weights.begin(), content->activeId);
    switch (xformContent.op) {
        case core::UPDATED:
            break;
        case core::ADDED:
            content->weights.insert(it, xformContent.xform->weight);
            break;
        case core::REMOVED: {
            content->weights.erase(it);
            if (content->activeId == content->weights.size()) {
                content->activeId--;
            }
            break;
        }
    }
    update();
}

void WeightsModel::handleAddXform() {
    xformAdded(content->weights.size());
}

void WeightsModel::handleRemoveXform() {
    xformRemoved(content->activeId);
}

void WeightsModel::getValues(vector<wxVector<wxVariant>>& data) const {
    if (!content.has_value()) {
        return;
    }
    int sz = content->weights.size();
    for (int i=0; i<sz; i++) {
        wxVector<wxVariant> row;
        row.push_back(to_string(i+1));
        row.push_back(to_string(content->weights[i]));
        data.push_back(row);
    }
}

void WeightsModel::setValue(const wxVariant& val, int row, int col) {
    if (!content.has_value()) {
        return;
    }
    if (col == 0) {
        update();
        return;
    }
    double oldValue = content->weights[row];
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
    content->weights[row] = newValue;
    ActiveXFormUpdateContent updateContent;
    updateContent.weight = newValue;
    xformUpdate(updateContent);
    update();
}

void WeightsModel::afterUpdate(int selectedRow) {
    if (!content.has_value()) {
        addXformButton->Disable();
        removeXformButton->Disable();
        return;
    }
    addXformButton->Enable();
    if (content->activeId != -1) {
        removeXformButton->Enable();
        SelectionViewModel::afterUpdate(content->activeId);
    } else {
        removeXformButton->Disable();
    }
}

}