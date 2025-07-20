#pragma once

#include "content.hpp"
#include "selection_view_model.hpp"
#include "../core/flame.hpp"
#include <memory>
#include <boost/signals2.hpp>
#include <wx/bmpbuttn.h>

namespace ui {

class WeightsModel: public SelectionViewModel {
public:
    WeightsModel(wxDataViewListCtrl* weightsListCtrl, wxBitmapButton* addXformButton,
        wxBitmapButton* removeXformButton);
    void handleSelectionEvent(wxDataViewEvent& event);
    void handleContent(WeightsContent content);
    void handleAddXform();
    void handleRemoveXform();
    boost::signals2::signal<void (int)> xformSelected;
    boost::signals2::signal<void ()> xformAdded;
    boost::signals2::signal<void (WeightsContent)> weightsChanged;
private:
    void getValues(std::vector<wxVector<wxVariant>>& data) const override;
    void setValue(const wxVariant& value, int row, int col) override;
    void afterUpdate(int selectedRow) override;

    wxBitmapButton* addXformButton;
    wxBitmapButton* removeXformButton;
    WeightsContent content;
    bool blockSelectionEvents;
};

}