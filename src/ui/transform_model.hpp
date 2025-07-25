#pragma once

#include "content.hpp"
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
    void handleNoContent();
    void handleFlameContent(std::optional<FlameContent> flameContent);
    void handleContent(CoefsContent content);
    boost::signals2::signal<void (CoefsContent)> contentChanged;
private:
    void getValues(std::vector<wxVector<wxVariant>>& data) const override;
    void setValue(const wxVariant& value, int row, int col) override;
    void afterUpdate(int selectedRow) override;

    wxButton* resetButton;
    bool noContent, accessCoefs;
    CoefsContent content;
};

}