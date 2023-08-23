#pragma once

#include "view_model.hpp"
#include "../core/flame.hpp"
#include <memory>
#include <boost/signals2.hpp>

namespace ui {

class FrameModel: public ViewModel {
public:
    FrameModel(std::shared_ptr<core::Flame> flame, wxDataViewListCtrl* frameListCtrl);
    boost::signals2::signal<void ()> frameChanged;
private:
    void getValues(std::vector<wxVector<wxVariant>>& data) const override;
    void setValue(const wxVariant& value, int row, int col) override;
    std::shared_ptr<core::Flame> flame;
};

}