#include "transform_model.hpp"
#include "content.hpp"
#include <optional>
#include <stdexcept>
#include <string>

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
    content = getCoefsContent(flameContent.xforms[0]);
    update();
}

void TransformModel::handleActiveXformContent(ActiveXFormContent xformContent) {
    content = getCoefsContent(xformContent.xform);
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

CoefsContent TransformModel::getCoefsContent(XFormContent xformContent) {
    CoefsContent coefsContent;
    if (accessCoefs) {
        coefsContent.ox = xformContent.preCoefs.ox;
        coefsContent.oy = xformContent.preCoefs.oy;
        coefsContent.xx = xformContent.preCoefs.xx;
        coefsContent.xy = xformContent.preCoefs.xy;
        coefsContent.yx = xformContent.preCoefs.yx;
        coefsContent.yy = xformContent.preCoefs.yy;
    } else {
        coefsContent.ox = xformContent.postCoefs.ox;
        coefsContent.oy = xformContent.postCoefs.oy;
        coefsContent.xx = xformContent.postCoefs.xx;
        coefsContent.xy = xformContent.postCoefs.xy;
        coefsContent.yx = xformContent.postCoefs.yx;
        coefsContent.yy = xformContent.postCoefs.yy;
    }
    return coefsContent;
}

}