#pragma once

#include "view_model.hpp"
#include "../core/flame.hpp"
#include <memory>
#include <wx/dataview.h>
#include <boost/signals2.hpp>

namespace ui {

class TransformModel: public ViewModel {
public:
    TransformModel(std::shared_ptr<core::Flame> flame, wxDataViewListCtrl* transformCtrl,
        bool accessCoefs);
    void handleActiveXformChanged(int id);
    void handleReset();
    boost::signals2::signal<void ()> transformCoordsChanged;
private:
    void getValues(std::vector<wxVector<wxVariant>>& data) const override;
    void setValue(const wxVariant& value, int row, int col) override;

    std::shared_ptr<core::Flame> flame;
    bool accessCoefs;
    int activeTransform;
};

}