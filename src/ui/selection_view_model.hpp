#pragma once

#include "view_model.hpp"

namespace ui {

class SelectionViewModel: public ViewModel {
public:
    SelectionViewModel(wxDataViewListCtrl* dvListCtrl);
    void handleKillFocusEvent(wxFocusEvent& event) override;
protected:
    virtual void afterUpdate(int selectedRow) override;
};

}