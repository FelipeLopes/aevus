#include "view_model.hpp"
#include <wx-3.2/wx/variant.h>

using std::vector;

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
    vector<wxVector<wxVariant>> data;
    getValues(data);
    for (int i=0; i<data.size(); i++) {
        dvListCtrl->AppendItem(data[i]);
    }
    afterUpdate(row);
    updateOngoing = false;
}

bool ViewModel::updating() {
    return updateOngoing;
}

void ViewModel::afterUpdate(int selectedRow) { }

void ViewModel::getValues(vector<wxVector<wxVariant>>& data) const { }

void ViewModel::setValue(const wxVariant& value, int row, int col) {
    update();
}

void ViewModel::selectRow(int row) {
    if (dvListCtrl->GetItemCount() > 0) {
        if (row != wxNOT_FOUND) {
            dvListCtrl->SelectRow(row);
        } else {
            dvListCtrl->SelectRow(0);
        }
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