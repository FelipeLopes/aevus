#include "view_model.hpp"

namespace ui {

ViewModel::ViewModel(wxDataViewListCtrl* dvListCtrl_): dvListCtrl(dvListCtrl_) {
    clearCtrl();
}

ViewModel::~ViewModel() { }

void ViewModel::update() {
    int row = dvListCtrl->GetSelectedRow();
    clearCtrl();
    int numCols = dvListCtrl->GetColumnCount();
    int numRows = getCount();
    for (int i=0; i<numRows; i++) {
        wxVector<wxVariant> data;
        for (int j=0; j<numCols; j++) {
            data.push_back(getValue(i, j));
        }
        dvListCtrl->AppendItem(data);
    }
    if (row != wxNOT_FOUND) {
        dvListCtrl->SelectRow(row);
    }
}

void ViewModel::handleValueChangedEvent(wxDataViewEvent &event) {
    auto item = event.GetItem();
    int row = dvListCtrl->ItemToRow(item);
    int col = event.GetColumn();
    auto value = dvListCtrl->GetTextValue(row, col);
    setValue(value, row, col);
}

void ViewModel::clearCtrl() {
    // HACK: we delete each item individually instead of calling DeleteAllItems()
    // because DeleteAllItems seems to be bugged, it changes the configuration
    // of the data view ctrl to enable the search textbox in Ubuntu, and we don't want it.
    for (int i=dvListCtrl->GetItemCount(); i>0; i--) {
        dvListCtrl->DeleteItem(i-1);
    }
}

}