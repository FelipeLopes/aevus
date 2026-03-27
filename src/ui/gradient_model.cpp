#include "gradient_model.hpp"
#include <wx-3.2/wx/variant.h>
#include <wx/debug.h>
#include <wx/log.h>

namespace ui {

GradientModel::GradientModel() {
    modelNode.title = "my_val";
    modelNode.value = 67;
}

bool GradientModel::IsContainer(const wxDataViewItem& item) const {
    return !item.IsOk();
}

wxDataViewItem GradientModel::GetParent(const wxDataViewItem& item) const {
    return wxDataViewItem(NULL);
};

unsigned GradientModel::GetChildren(const wxDataViewItem& item, wxDataViewItemArray& children) const  {
    GradientModelNode* node = static_cast<GradientModelNode*>(item.GetID());
    if (node == NULL) {
        children.Add(wxDataViewItem((void*)&modelNode));
        return 1;
    } else {
        return 0;
    }
}

void GradientModel::GetValue(wxVariant& variant, const wxDataViewItem& item, unsigned col) const {
    wxASSERT(item.IsOk());
    GradientModelNode* node = static_cast<GradientModelNode*>(item.GetID());
    switch (col) {
        case 0: variant = node->title; break;
        case 1: variant = (long)node->value; break;
        default: wxLogError("GradientModel::GetValue wrong column %d", col);
    } 
}

bool GradientModel::SetValue(const wxVariant& variant, const wxDataViewItem& item, unsigned col) {
    wxASSERT(item.IsOk());
    GradientModelNode* node = static_cast<GradientModelNode*>(item.GetID());
    switch (col) {
        case 0:
            node->title = variant.GetString();
            return true;
        case 1: 
            node->value = variant.GetLong();
            return true;
        default: 
            wxLogError("GradientModel::SetValue wrong column %d", col);
            return false;
    }
}

}