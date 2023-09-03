#include "transform_model.hpp"
#include <stdexcept>
#include <string>

using core::Flame;
using std::shared_ptr;
using std::string;
using std::to_string;
using std::vector;

namespace ui {

TransformModel::TransformModel(shared_ptr<Flame> flame_, wxDataViewListCtrl* transformCtrl,
    bool accessCoefs_): ViewModel(transformCtrl), flame(flame_), accessCoefs(accessCoefs_),
    activeTransform(0) { }

void TransformModel::handleActiveXformChanged(int id) {
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

void TransformModel::getValues(vector<wxVector<wxVariant>>& data) const {
    if (activeTransform == -1) {
        return;
    }
    core::Affine* aff = NULL;
    if (accessCoefs) {
        aff = flame->xforms.get(activeTransform)->coefs.get();
    } else {
        aff = flame->xforms.get(activeTransform)->post.get();
    }
    wxVector<wxVariant> firstRow;
    firstRow.push_back("X");
    firstRow.push_back(to_string(aff->xx));
    firstRow.push_back(to_string(aff->xy));
    wxVector<wxVariant> secondRow;
    secondRow.push_back("Y");
    secondRow.push_back(to_string(aff->yx));
    secondRow.push_back(to_string(aff->yy));
    wxVector<wxVariant> thirdRow;
    thirdRow.push_back("O");
    thirdRow.push_back(to_string(aff->ox));
    thirdRow.push_back(to_string(aff->oy));

    data.push_back(firstRow);
    data.push_back(secondRow);
    data.push_back(thirdRow);
}

void TransformModel::setValue(const wxVariant& val, int row, int col) {
    if (col == 0) {
        update();
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