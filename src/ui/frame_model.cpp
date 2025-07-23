#include "frame_model.hpp"
#include "view_model.hpp"

using std::string;
using std::to_string;

namespace ui {

FrameModel::FrameModel(wxDataViewListCtrl* frameListCtrl): ViewModel(frameListCtrl) {
    content.flameLoaded = false;
    update();
}

void FrameModel::handleContent(FrameContent content) {
    this->content = content;
    update();
}

void FrameModel::getValues(std::vector<wxVector<wxVariant>>& data) const {
    if (!content.flameLoaded) {
        return;
    }
    wxVector<wxVariant> row;
    row.push_back("width");
    row.push_back(to_string(content.flameSize.width));
    data.push_back(row);
    row.clear();
    row.push_back("height");
    row.push_back(to_string(content.flameSize.height));
    data.push_back(row);
    row.clear();
    row.push_back("center x");
    row.push_back(to_string(content.flameCenter.x));
    data.push_back(row);
    row.clear();
    row.push_back("center y");
    row.push_back(to_string(content.flameCenter.y));
    data.push_back(row);
    row.clear();
    row.push_back("scale");
    row.push_back(to_string(content.flameScale));
    data.push_back(row);
}

void FrameModel::setValue(const wxVariant& val, int row, int col) {
    if (col == 0) {
        update();
        return;
    }
    string text = val.GetString().ToStdString();
    /*
    if (row < 2) {
        int oldValue = 0;
        switch (row) {
            case 0: oldValue = flame->size.value().width; break;
            case 1: oldValue = flame->size.value().height; break;
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
            case 0: flame->size.get()->width = newValue; break;
            case 1: flame->size.get()->height = newValue; break;
        }
        frameChanged();
    } else {
        double oldValue = 0;
        switch (row) {
            case 2: oldValue = flame->center.value().x; break;
            case 3: oldValue = flame->center.value().y; break;
            case 4: oldValue = flame->scale.value(); break;
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
            case 2: flame->center.get()->x = newValue; break;
            case 3: flame->center.get()->y = newValue; break;
            case 4: flame->scale.setValue(newValue); break;
        }
        frameChanged();
    }*/
}

}