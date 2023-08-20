#pragma once

#include "view_model.hpp"
#include "../core/flame.hpp"
#include <memory>
#include <wx/dataview.h>
#include <boost/signals2.hpp>

namespace ui {

class TransformModel: public ViewModel {
public:
    TransformModel(std::shared_ptr<core::Flame> flame, wxWindow* eventHandler,
        wxDataViewListCtrl* transformCtrl, bool accessCoefs);
    void handleActiveFormChangedEvent(wxCommandEvent& event);
    void handleReset();
    boost::signals2::signal<void ()> transformValueChanged;
private:
    int getCount() const override;
    wxVariant getValue(int row, int col) const override;
    void setValue(const wxVariant& value, int row, int col) override;

    std::shared_ptr<core::Flame> flame;
    int activeTransform;
    wxWindow* eventHandler;
    bool accessCoefs;
    void fireFlameUpdateEvent();
};

}