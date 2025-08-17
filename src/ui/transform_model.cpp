#include "transform_model.hpp"
#include <optional>
#include <stdexcept>
#include <string>

using core::FlameContent;
using core::ActiveXFormUpdateContent;
using core::ActiveXFormContent;
using core::CoefsContent;
using core::XFormContent;
using std::string;
using std::to_string;
using std::vector;

namespace ui {

TransformModel::TransformModel(wxDataViewListCtrl* transformCtrl,
    wxButton* resetButton_, bool accessCoefs_): ViewModel(transformCtrl),
    resetButton(resetButton_), accessCoefs(accessCoefs_)
{
    resetButton->Disable();
}

void TransformModel::handleReset() {
    content->xx = 1;
    content->xy = 0;
    content->yx = 0;
    content->yy = 1;
    content->ox = 0;
    content->oy = 0;
    ActiveXFormUpdateContent updateContent;
    if (accessCoefs) {
        updateContent.preCoefs = content;
    } else {
        updateContent.postCoefs = content;
    }
    xformUpdate(updateContent);
}

void TransformModel::handleFlameContent(std::optional<FlameContent> flameContent_) {
    if (!flameContent_.has_value()) {
        content = std::nullopt;
        update();
        return;
    }
    content = CoefsContent();
    auto flameContent = flameContent_.value();
    if (flameContent.xforms.size() == 0) {
        update();
        return;
    }
    updateContent(flameContent.xforms[0]);
    update();
}

void TransformModel::handleActiveXformContent(ActiveXFormContent xformContent) {
    updateContent(xformContent.xform);
    update();
}

void TransformModel::getValues(vector<wxVector<wxVariant>>& data) const {
    if (!content.has_value()) {
        return;
    }
    wxVector<wxVariant> firstRow;
    firstRow.push_back("X");
    firstRow.push_back(to_string(content->xx));
    firstRow.push_back(to_string(content->xy));
    wxVector<wxVariant> secondRow;
    secondRow.push_back("Y");
    secondRow.push_back(to_string(content->yx));
    secondRow.push_back(to_string(content->yy));
    wxVector<wxVariant> thirdRow;
    thirdRow.push_back("O");
    thirdRow.push_back(to_string(content->ox));
    thirdRow.push_back(to_string(content->oy));

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
    switch (num) {
        case 0: oldValue = content->xx; break;
        case 1: oldValue = content->xy; break;
        case 2: oldValue = content->yx; break;
        case 3: oldValue = content->yy; break;
        case 4: oldValue = content->ox; break;
        case 5: oldValue = content->oy; break;
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
        case 0: content->xx = newValue; break;
        case 1: content->xy = newValue; break;
        case 2: content->yx = newValue; break;
        case 3: content->yy = newValue; break;
        case 4: content->ox = newValue; break;
        case 5: content->oy = newValue; break;
        default: throw std::invalid_argument("Invalid cell");
    }
    ActiveXFormUpdateContent updateContent;
    if (accessCoefs) {
        updateContent.preCoefs = content;
    } else {
        updateContent.postCoefs = content;
    }
    xformUpdate(updateContent);
}

void TransformModel::afterUpdate(int selectedRow) {
    if (content.has_value()) {
        resetButton->Enable();
    } else {
        resetButton->Disable();
    }
}

void TransformModel::updateContent(std::optional<XFormContent> xformContent) {
    if (!xformContent.has_value()) {
        content = std::nullopt;
        return;
    }
    auto xformContentValue = xformContent.value();
    content = CoefsContent();
    if (accessCoefs) {
        content->ox = xformContentValue.preCoefs.ox;
        content->oy = xformContentValue.preCoefs.oy;
        content->xx = xformContentValue.preCoefs.xx;
        content->xy = xformContentValue.preCoefs.xy;
        content->yx = xformContentValue.preCoefs.yx;
        content->yy = xformContentValue.preCoefs.yy;
    } else {
        content->ox = xformContentValue.postCoefs.ox;
        content->oy = xformContentValue.postCoefs.oy;
        content->xx = xformContentValue.postCoefs.xx;
        content->xy = xformContentValue.postCoefs.xy;
        content->yx = xformContentValue.postCoefs.yx;
        content->yy = xformContentValue.postCoefs.yy;
    }
}

}