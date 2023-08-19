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
    virtual int getCount() const = 0;
    virtual wxVariant getValue(int row, int col) const = 0;
    virtual void setValue(const wxVariant& value, int row, int col) = 0;
private:
    wxDataViewListCtrl* dvListCtrl;
    void clearCtrl();
};

}