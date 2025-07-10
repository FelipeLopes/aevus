#pragma once

#include "view_model.hpp"
#include <wx/button.h>
#include <wx/dataview.h>
#include <boost/signals2.hpp>

namespace ui {

class TransformModel: public ViewModel {
public:
    TransformModel(wxDataViewListCtrl* transformCtrl, wxButton* resetButton, bool accessCoefs);
    void handleActiveXformChanged(int id);
    void handleReset();
    boost::signals2::signal<void ()> transformCoordsChanged;
private:
    void getValues(std::vector<wxVector<wxVariant>>& data) const override;
    void setValue(const wxVariant& value, int row, int col) override;
    void afterUpdate(int selectedRow) override;

    core::Flame* flame;
    wxButton* resetButton;
    bool accessCoefs;
    int activeTransform;
};

}