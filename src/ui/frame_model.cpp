#include "frame_model.hpp"
#include "view_model.hpp"

using std::string;
using std::to_string;

namespace ui {

FrameModel::FrameModel(wxDataViewListCtrl* frameListCtrl): ViewModel(frameListCtrl) {
    update();
}

void FrameModel::handleFlameContent(std::optional<FlameContent> flameContent_) {
    if (!flameContent_.has_value()) {
        content = std::nullopt;
        update();
        return;
    }
    content = FrameContent();
    auto flameContent = flameContent_.value();
    content->flameSize = flameContent.frame.flameSize;
    content->flameCenter = flameContent.frame.flameCenter;
    content->flameScale = flameContent.frame.flameScale;
    update();
}

void FrameModel::getValues(std::vector<wxVector<wxVariant>>& data) const {
    if (!content.has_value()) {
        return;
    }
    wxVector<wxVariant> row;
    row.push_back("width");
    row.push_back(to_string(content->flameSize.width));
    data.push_back(row);
    row.clear();
    row.push_back("height");
    row.push_back(to_string(content->flameSize.height));
    data.push_back(row);
    row.clear();
    row.push_back("center x");
    row.push_back(to_string(content->flameCenter.x));
    data.push_back(row);
    row.clear();
    row.push_back("center y");
    row.push_back(to_string(content->flameCenter.y));
    data.push_back(row);
    row.clear();
    row.push_back("scale");
    row.push_back(to_string(content->flameScale));
    data.push_back(row);
}

void FrameModel::setValue(const wxVariant& val, int row, int col) {
    if (!content.has_value() || col == 0) {
        update();
        return;
    }
    string text = val.GetString().ToStdString();
    if (row < 2) {
        int oldValue = 0;
        switch (row) {
            case 0: oldValue = content->flameSize.width; break;
            case 1: oldValue = content->flameSize.height; break;
        }
        int newValue = 0;
        try {
            newValue = std::stoi(text);
        } catch (std::invalid_argument& e) {
            update();
            return;
        }
        if (newValue == oldValue) {
            update();
            return;
        }
        switch (row) {
            case 0: content->flameSize.width = newValue; break;
            case 1: content->flameSize.height = newValue; break;
        }
        frameContent(content.value());
    } else {
        double oldValue = 0;
        switch (row) {
            case 2: oldValue = content->flameCenter.x; break;
            case 3: oldValue = content->flameCenter.y; break;
            case 4: oldValue = content->flameScale; break;
        }
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
        switch (row) {
            case 2: content->flameCenter.x = newValue; break;
            case 3: content->flameCenter.y = newValue; break;
            case 4: content->flameScale = newValue; break;
        }
        frameContent(content.value());
    }
    update();
}

}