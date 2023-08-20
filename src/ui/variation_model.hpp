#pragma once

#include "view_model.hpp"
#include "../core/flame.hpp"
#include <memory>

namespace ui {

class VariationModel: public ViewModel {
public:
    VariationModel(std::shared_ptr<core::Flame> flame, wxDataViewListCtrl* variationCtrl);
    void handleActiveXformChanged(int id);
private:
    void getValues(std::vector<wxVector<wxVariant>>& data) const override;

    std::shared_ptr<core::Flame> flame;
    int activeTransform;
};

}