#include "selection_view_model.hpp"

namespace ui {

SelectionViewModel::SelectionViewModel(wxDataViewListCtrl* dvListCtrl): ViewModel(dvListCtrl) { }

void SelectionViewModel::handleKillFocusEvent(wxFocusEvent &event) { }

}