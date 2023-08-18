#include "transform_model.hpp"
#include "aevus_frame.hpp"
#include "view_model.hpp"
#include <stdexcept>
#include <string>

using std::to_string;

namespace ui {

TransformModel::TransformModel(wxWindow* eventHandler_, wxDataViewListCtrl* transformCtrl):
    ViewModel(transformCtrl), eventHandler(eventHandler_) { }

int TransformModel::getCount() const {
    return 3;
}

wxVariant TransformModel::getValue(int row, int col) const {
    if (col == 0) {
        switch (row) {
            case 0: return "X";
            case 1: return "Y";
            case 2: return "O";
            default: throw std::invalid_argument("Invalid row");
        }
    }
    if ((row == 0 && col == 1) || (row == 1 && col == 2)) {
        return to_string(1.0);
    }
    return to_string(0.0);
}

void TransformModel::setValue(const wxVariant& val, int row, int col) {
    printf("setValue called\n");
    fireFlameUpdateEvent();
}

void TransformModel::fireFlameUpdateEvent() {
    wxCommandEvent flameUpdateEvent(FLAME_UPDATE_EVENT, wxID_ANY);
    flameUpdateEvent.SetEventObject(eventHandler);
    eventHandler->ProcessWindowEvent(flameUpdateEvent);
}

}