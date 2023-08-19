#pragma once

#include "view_model.hpp"

namespace ui {

class SelectionViewModel: public ViewModel {
public:
    SelectionViewModel(wxDataViewListCtrl* dvListCtrl);
    void handleKillFocusEvent(wxFocusEvent& event) override;
private:
    void afterUpdate(int selectedRow) override;
};

}