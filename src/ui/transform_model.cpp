#include "transform_model.hpp"
#include "aevus_frame.hpp"
#include <stdexcept>
#include <string>

using core::Flame;
using std::shared_ptr;
using std::to_string;

namespace ui {

TransformModel::TransformModel(shared_ptr<Flame> flame_, wxWindow* eventHandler_,
    wxDataViewListCtrl* transformCtrl, bool accessCoefs_):
    ViewModel(transformCtrl), flame(flame_),
    eventHandler(eventHandler_), accessCoefs(accessCoefs_)
{
    activeTransform = 0;
}

void TransformModel::handleActiveFormChangedEvent(wxCommandEvent& event) {
    activeTransform = event.GetInt();
    update();
}

void TransformModel::handleReset() {
    bool changing = false;
    core::Affine* aff = NULL;
    if (accessCoefs) {
        aff = flame->xforms.get(activeTransform)->coefs.get();
    } else {
        aff = flame->xforms.get(activeTransform)->post.get();
    }
    changing |= (aff->xx != 1);
    changing |= (aff->xy != 0);
    changing |= (aff->yx != 0);
    changing |= (aff->yy != 1);
    changing |= (aff->ox != 0);
    changing |= (aff->oy != 0);
    if (changing) {
        aff->xx = 1;
        aff->xy = 0;
        aff->yx = 0;
        aff->yy = 1;
        aff->ox = 0;
        aff->oy = 0;
        transformValueChanged();
    }
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
    core::Affine* aff = NULL;
    if (accessCoefs) {
        aff = flame->xforms.get(activeTransform)->coefs.get();
    } else {
        aff = flame->xforms.get(activeTransform)->post.get();
    }
    int num = 2*row + col - 1;
    switch (num) {
        case 0: return to_string(aff->xx);
        case 1: return to_string(aff->xy);
        case 2: return to_string(aff->yx);
        case 3: return to_string(aff->yy);
        case 4: return to_string(aff->ox);
        case 5: return to_string(aff->oy);
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
        update();
        return;
    }
    core::Affine* aff = NULL;
    if (accessCoefs) {
        aff = flame->xforms.get(activeTransform)->coefs.get();
    } else {
        aff = flame->xforms.get(activeTransform)->post.get();
    }
    int num = 2*row + col - 1;
    double oldVal = 0;
    switch (num) {
        case 0: oldVal = aff->xx; aff->xx = value; break;
        case 1: oldVal = aff->xy; aff->xy = value; break;
        case 2: oldVal = aff->yx; aff->yx = value; break;
        case 3: oldVal = aff->yy; aff->yy = value; break;
        case 4: oldVal = aff->ox; aff->ox = value; break;
        case 5: oldVal = aff->oy; aff->oy = value; break;
        default: throw std::invalid_argument("Invalid cell");
    }
    if (oldVal != value) {
        transformValueChanged();
    }
}

void TransformModel::fireFlameUpdateEvent() {
    wxCommandEvent flameUpdateEvent(FLAME_UPDATE_EVENT, wxID_ANY);
    flameUpdateEvent.SetEventObject(eventHandler);
    eventHandler->ProcessWindowEvent(flameUpdateEvent);
}

}