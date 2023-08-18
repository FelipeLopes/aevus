#include "transform_model.hpp"
#include "aevus_frame.hpp"
#include "view_model.hpp"
#include <stdexcept>
#include <string>

using core::Flame;
using std::shared_ptr;
using std::to_string;

namespace ui {

TransformModel::TransformModel(shared_ptr<Flame> flame_, wxWindow* eventHandler_,
    wxDataViewListCtrl* transformCtrl):
    ViewModel(transformCtrl), flame(flame_), eventHandler(eventHandler_)
{
    activeTransform = 0;
}

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
    auto coefs = flame->xforms.get(activeTransform)->coefs.get();
    int num = 2*row + col - 1;
    switch (num) {
        case 0: return to_string(coefs->xx);
        case 1: return to_string(coefs->xy);
        case 2: return to_string(coefs->yx);
        case 3: return to_string(coefs->yy);
        case 4: return to_string(coefs->ox);
        case 5: return to_string(coefs->oy);
        default: throw std::invalid_argument("Invalid cell");
    }
}

void TransformModel::setValue(const wxVariant& val, int row, int col) {
    if (col == 0) {
        return;
    }
    double value = 0;
    try {
        value = std::stod(val.GetString().ToStdString());
    } catch (std::invalid_argument& e) {
        return;
    }
    auto coefs = flame->xforms.get(activeTransform)->coefs.get();
    int num = 2*row + col - 1;
    switch (num) {
        case 0: coefs->xx = value; break;
        case 1: coefs->xy = value; break;
        case 2: coefs->yx = value; break;
        case 3: coefs->yy = value; break;
        case 4: coefs->ox = value; break;
        case 5: coefs->oy = value; break;
        default: throw std::invalid_argument("Invalid cell");
    }
    fireFlameUpdateEvent();
}

void TransformModel::fireFlameUpdateEvent() {
    wxCommandEvent flameUpdateEvent(FLAME_UPDATE_EVENT, wxID_ANY);
    flameUpdateEvent.SetEventObject(eventHandler);
    eventHandler->ProcessWindowEvent(flameUpdateEvent);
}

}