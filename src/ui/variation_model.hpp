#pragma once

#include "view_model.hpp"
#include "../core/flame.hpp"
#include <memory>
#include <boost/signals2.hpp>

namespace ui {

class VariationModel: public ViewModel {
public:
    VariationModel(std::shared_ptr<core::Flame> flame, wxDataViewListCtrl* variationCtrl);
    void handleActiveXformChanged(int id);
    boost::signals2::signal<void ()> variationDataChanged;
private:
    void getValues(std::vector<wxVector<wxVariant>>& data) const override;
    void setValue(const wxVariant& value, int row, int col) override;

    std::shared_ptr<core::Flame> flame;
    int activeTransform;
};

}