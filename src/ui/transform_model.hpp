#pragma once

#include "../core/content.hpp"
#include "view_model.hpp"
#include <wx/button.h>
#include <wx/dataview.h>
#include <boost/signals2.hpp>

namespace ui {

class TransformModel: public ViewModel {
public:
    TransformModel(wxDataViewListCtrl* transformCtrl, wxButton* resetButton, bool accessCoefs);
    void handleReset();
    void handleFlameContent(std::optional<core::FlameContent> flameContent);
    void handleActiveXformContent(core::ActiveXFormContent xformContent);
    boost::signals2::signal<void (core::ActiveXFormUpdateContent)> xformUpdate;
private:
    void getValues(std::vector<wxVector<wxVariant>>& data) const override;
    void setValue(const wxVariant& value, int row, int col) override;
    void afterUpdate(int selectedRow) override;
    void updateContent(std::optional<core::XFormContent> xformContent);

    wxButton* resetButton;
    bool accessCoefs;
    std::optional<core::CoefsContent> content;
};

}