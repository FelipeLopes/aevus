#include "weights_model.hpp"
#include "aevus_frame.hpp"
#include "selection_view_model.hpp"
#include <string>

using core::Flame;
using std::shared_ptr;
using std::to_string;

namespace ui {

WeightsModel::WeightsModel(shared_ptr<Flame> flame_, wxWindow* eventHandler_,
    wxDataViewListCtrl* weightsListCtrl): SelectionViewModel(weightsListCtrl),
    flame(flame_), eventHandler(eventHandler_) { }

void WeightsModel::handleSelectionEvent(wxDataViewEvent& event) {
    if (!updating()) {
        fireFlameXformChangeEvent();
    }
}

void WeightsModel::fireFlameXformChangeEvent() {
    wxCommandEvent flameXformChangeEvent(FLAME_XFORM_CHANGE_EVENT, wxID_ANY);
    flameXformChangeEvent.SetEventObject(eventHandler);
    flameXformChangeEvent.SetInt(getSelectedRow());
    eventHandler->ProcessWindowEvent(flameXformChangeEvent);
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
    update();
}

}