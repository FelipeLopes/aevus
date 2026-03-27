#pragma once

#include <wx/dataview.h>

namespace ui {

struct GradientModelNode {
    std::string title;
    int value;
};

class GradientModel: public wxDataViewModel {
public:
    GradientModel();
    bool IsContainer(const wxDataViewItem& item) const override;
    wxDataViewItem GetParent(const wxDataViewItem& item) const override;
    unsigned GetChildren(const wxDataViewItem& item, wxDataViewItemArray& children) const override;
    void GetValue(wxVariant& variant, const wxDataViewItem& item, unsigned col) const override;
    bool SetValue(const wxVariant& variant, const wxDataViewItem& item, unsigned col) override;
private:
    GradientModelNode modelNode;
};

}