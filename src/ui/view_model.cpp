#include "view_model.hpp"

namespace ui {

ViewModel::ViewModel(wxDataViewListCtrl* dvListCtrl_): dvListCtrl(dvListCtrl_) {
    clearCtrl();
}

ViewModel::~ViewModel() { }

void ViewModel::update() {
    updateCtrl();
}

void ViewModel::receiveValueChangedEvent(wxDataViewEvent &event) {
    auto item = event.GetItem();
    int row = dvListCtrl->ItemToRow(item);
    int col = event.GetColumn();
    auto value = event.GetValue();
    setValue(value, row, col);
    updateCtrl();
}

void ViewModel::clearCtrl() {
    // HACK: we delete each item individually instead of calling DeleteAllItems()
    // because DeleteAllItems seems to be bugged, it changes the configuration
    // of the data view ctrl to enable the search textbox in Ubuntu, and we don't want it.
    for (int i=dvListCtrl->GetItemCount(); i>0; i--) {
        dvListCtrl->DeleteItem(i-1);
    }
}

void ViewModel::updateCtrl() {
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
}

}