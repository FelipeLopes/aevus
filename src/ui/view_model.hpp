#pragma once

#include <wx/variant.h>
#include <wx/dataview.h>

namespace ui {

class ViewModel {
public:
    ViewModel(wxDataViewListCtrl* dvListCtrl);
    void handleValueChangedEvent(wxDataViewEvent& event);
    virtual void handleKillFocusEvent(wxFocusEvent& event);
    void update();
    virtual ~ViewModel();
protected:
    virtual void getValues(std::vector<wxVector<wxVariant>>& data) const;
    virtual void setValue(const wxVariant& value, int row, int col);
    virtual void afterUpdate(int selectedRow);
    void selectRow(int row);
    int getSelectedRow();
    bool updating();
private:
    wxDataViewListCtrl* dvListCtrl;
    bool updateOngoing;
    void clearCtrl();
};

}