#pragma once

#include "selection_view_model.hpp"
#include "../core/flame.hpp"
#include <memory>
#include <boost/signals2.hpp>

namespace ui {

class WeightsModel: public SelectionViewModel {
public:
    WeightsModel(std::shared_ptr<core::Flame> flame, wxDataViewListCtrl* weightsListCtrl);
    void handleSelectionEvent(wxDataViewEvent& event);
    boost::signals2::signal<void (int)> xformSelected;
    boost::signals2::signal<void ()> weightsChanged;
private:
    int getCount() const override;
    wxVariant getValue(int row, int col) const override;
    void setValue(const wxVariant& value, int row, int col) override;

    std::shared_ptr<core::Flame> flame;
};

}