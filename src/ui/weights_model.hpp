#pragma once

#include "selection_view_model.hpp"
#include "../core/flame.hpp"
#include <memory>

namespace ui {

class WeightsModel: public SelectionViewModel {
public:
    WeightsModel(std::shared_ptr<core::Flame> flame, wxWindow* eventHandler,
        wxDataViewListCtrl* weightsListCtrl);
    void handleSelectionEvent(wxDataViewEvent& event);
private:
    int getCount() const override;
    wxVariant getValue(int row, int col) const override;
    void setValue(const wxVariant& value, int row, int col) override;

    void fireFlameXformChangeEvent();

    std::shared_ptr<core::Flame> flame;
    wxWindow* eventHandler;
};

}