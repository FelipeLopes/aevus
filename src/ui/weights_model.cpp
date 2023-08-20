#include "weights_model.hpp"
#include "selection_view_model.hpp"
#include <string>

using core::Flame;
using std::shared_ptr;
using std::string;
using std::to_string;

namespace ui {

WeightsModel::WeightsModel(shared_ptr<Flame> flame_, wxDataViewListCtrl* weightsListCtrl):
    SelectionViewModel(weightsListCtrl), flame(flame_) { }

void WeightsModel::handleSelectionEvent(wxDataViewEvent& event) {
    if (!updating()) {
        xformSelected(getSelectedRow());
    }
}

int WeightsModel::getCount() const {
    return flame->xforms.size();
}

wxVariant WeightsModel::getValue(int row, int col) const {
    if (col == 0) {
        return to_string(row + 1);
    }
    return to_string(flame->xforms.get(row)->weight.getValue());
}

void WeightsModel::setValue(const wxVariant& val, int row, int col) {
    if (col == 0) {
        return;
    }
    double oldValue = flame->xforms.get(row)->weight.getValue();
    string text = val.GetString().ToStdString();
    if (text == to_string(oldValue)) {
        return;
    }
    double newValue = 0;
    try {
        newValue = std::stod(val.GetString().ToStdString());
    } catch (std::invalid_argument& e) {
        update();
        return;
    }
    flame->xforms.get(row)->weight.setValue(newValue);
    weightsChanged();
}

}