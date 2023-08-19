#include "view_model.hpp"

namespace ui {

ViewModel::ViewModel(wxDataViewListCtrl* dvListCtrl_): dvListCtrl(dvListCtrl_),
    updateOngoing(false)
{
    clearCtrl();
}

ViewModel::~ViewModel() { }

void ViewModel::update() {
    updateOngoing = true;
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
    afterUpdate(row);
    updateOngoing = false;
}

bool ViewModel::updating() {
    return updateOngoing;
}

void ViewModel::afterUpdate(int selectedRow) { }

void ViewModel::setValue(const wxVariant& value, int row, int col) {
    update();
}

void ViewModel::selectRow(int row) {
    if (row != wxNOT_FOUND) {
        dvListCtrl->SelectRow(row);
    } else if (getCount() > 0) {
        dvListCtrl->SelectRow(0);
    }
}

int ViewModel::getSelectedRow() {
    return dvListCtrl->GetSelectedRow();
}

void ViewModel::handleValueChangedEvent(wxDataViewEvent &event) {
    auto item = event.GetItem();
    int row = dvListCtrl->ItemToRow(item);
    int col = event.GetColumn();
    auto value = dvListCtrl->GetTextValue(row, col);
    setValue(value, row, col);
}

void ViewModel::handleKillFocusEvent(wxFocusEvent& event) {
    int row = dvListCtrl->GetSelectedRow();
    if (row != wxNOT_FOUND) {
        dvListCtrl->UnselectRow(row);
    }
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