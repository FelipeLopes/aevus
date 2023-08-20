#include "transform_model.hpp"
#include <stdexcept>
#include <string>

using core::Flame;
using std::shared_ptr;
using std::string;
using std::to_string;

namespace ui {

TransformModel::TransformModel(shared_ptr<Flame> flame_, wxDataViewListCtrl* transformCtrl,
    bool accessCoefs_): ViewModel(transformCtrl), flame(flame_), accessCoefs(accessCoefs_),
    activeTransform(0) { }

void TransformModel::handleActiveFormChanged(int id) {
    activeTransform = id;
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
        transformCoordsChanged();
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
    int num = 2*row + col - 1;
    double oldValue = 0;
    core::Affine* aff = NULL;
    if (accessCoefs) {
        aff = flame->xforms.get(activeTransform)->coefs.get();
    } else {
        aff = flame->xforms.get(activeTransform)->post.get();
    }
    switch (num) {
        case 0: oldValue = aff->xx; break;
        case 1: oldValue = aff->xy; break;
        case 2: oldValue = aff->yx; break;
        case 3: oldValue = aff->yy; break;
        case 4: oldValue = aff->ox; break;
        case 5: oldValue = aff->oy; break;
        default: throw std::invalid_argument("Invalid cell");
    }
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
    switch (num) {
        case 0: aff->xx = newValue; break;
        case 1: aff->xy = newValue; break;
        case 2: aff->yx = newValue; break;
        case 3: aff->yy = newValue; break;
        case 4: aff->ox = newValue; break;
        case 5: aff->oy = newValue; break;
        default: throw std::invalid_argument("Invalid cell");
    }
    transformCoordsChanged();
}

}