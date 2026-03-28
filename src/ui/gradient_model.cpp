#include "gradient_model.hpp"
#include <wx/artprov.h>
#include <wx/dataview.h>
#include <wx/variant.h>
#include <wx/debug.h>
#include <wx/log.h>

namespace ui {

GradientModel::GradientModel() {
    mapping = {
        {
            "A", {
                {"January", 31},
                {"February", 28},
                {"March", 31}
            }
        },
        {
            "B", {
                {"April", 30},
                {"May", 31},
                {"June", 30}
            }
        },
        {
            "C", {
                {"July", 31},
                {"August", 31},
                {"September", 30}
            }
        },
        {
            "D", {
                {"October", 31},
                {"November", 30},
                {"December", 31}
            }
        }
    };
    for (auto it=mapping.begin(); it != mapping.end(); it++) {
        folders.emplace_back(std::make_unique<GradientContainerNode>(nullptr, it->first));
        for (int i=0; i<it->second.size(); i++) {
            folders.back()->addEntityChild(&it->second[i]);
        }
    }
}

bool GradientModel::IsContainer(const wxDataViewItem& item) const {
    auto node = static_cast<GradientModelNode*>(item.GetID());
    return (node == NULL) || (node->isContainer());
}

wxDataViewItem GradientModel::GetParent(const wxDataViewItem& item) const {
    auto node = static_cast<GradientModelNode*>(item.GetID());
    if (node == NULL) {
        return wxDataViewItem(NULL);
    }
    return wxDataViewItem(node->parent);
};

unsigned GradientModel::GetChildren(const wxDataViewItem& item, wxDataViewItemArray& children) const  {
    GradientModelNode* node = static_cast<GradientModelNode*>(item.GetID());
    if (node == NULL) {
        for (auto& fld: folders) {
            children.Add(wxDataViewItem(fld.get()));
        }
        return folders.size();
    } else if (auto container = dynamic_cast<GradientContainerNode*>(node)){
        for (auto& child: container->children) {
            children.Add(wxDataViewItem(child.get()));
        }
        return container->children.size();
    }
    return 0;
}

void GradientModel::GetValue(wxVariant& variant, const wxDataViewItem& item, unsigned col) const {
    wxASSERT(item.IsOk());
    GradientModelNode* node = static_cast<GradientModelNode*>(item.GetID());
    if (auto container = dynamic_cast<GradientContainerNode*>(node)) {
        auto icon = container->expanded ? wxArtProvider::GetIcon(wxART_FOLDER_OPEN) : wxArtProvider::GetIcon(wxART_FOLDER);
        switch (col) {
            case 0: variant << wxDataViewIconText(container->name, icon); break;
            case 1: /* do nothing */ break;
            default: wxLogError("GradientModel::GetValue wrong column %d", col);
        }
    } else if (auto leaf = dynamic_cast<GradientEntityNode*>(node)) {
        switch (col) {
            case 0: variant << wxDataViewIconText(leaf->entity->title); break;
            case 1: variant = (long)leaf->entity->value; break;
            default: wxLogError("GradientModel::GetValue wrong column %d", col);
        }
    } else {
        wxLogError("GradientModel::GetValue unknown GradientModelNode");
    }
}

bool GradientModel::SetValue(const wxVariant& variant, const wxDataViewItem& item, unsigned col) {
    wxASSERT(item.IsOk());
    GradientModelNode* node = static_cast<GradientModelNode*>(item.GetID());
    if (auto container = dynamic_cast<GradientContainerNode*>(node)) {
        switch (col) {
            case 0: {
                wxDataViewIconText iconText;
                iconText << variant;
                container->name = iconText.GetText();
                return true;
            }
            case 1: /* do nothing */
                return false;
            default:
                wxLogError("GradientModel::SetValue wrong column %d", col);
                return false;
        }
    } else if (auto leaf = dynamic_cast<GradientEntityNode*>(node)) {
        switch (col) {
            case 0: {
                wxDataViewIconText iconText;
                iconText << variant;
                leaf->entity->title = iconText.GetText();
                return true;
            }
            case 1:
                leaf->entity->value = variant.GetLong();
                return true;
            default:
                wxLogError("GradientModel::SetValue wrong column %d", col);
                return false;
        }
    } else {
        wxLogError("GradientModel::GetValue unknown GradientModelNode");
    }
    return false;
}

GradientDataViewCtrl::GradientDataViewCtrl(wxWindow* parent): wxDataViewCtrl(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0) {
    Connect( wxEVT_DATAVIEW_ITEM_EXPANDED, wxDataViewEventHandler( GradientDataViewCtrl::onExpanded ) );
    Connect( wxEVT_DATAVIEW_ITEM_COLLAPSED, wxDataViewEventHandler( GradientDataViewCtrl::onCollapsed ) );
}

GradientDataViewCtrl::~GradientDataViewCtrl() {
    Disconnect( wxEVT_DATAVIEW_ITEM_EXPANDED, wxDataViewEventHandler( GradientDataViewCtrl::onExpanded ) );
    Disconnect( wxEVT_DATAVIEW_ITEM_COLLAPSED, wxDataViewEventHandler( GradientDataViewCtrl::onCollapsed ) );
}

void GradientDataViewCtrl::onExpanded(wxDataViewEvent& event) {
    auto item = event.GetItem();
    wxASSERT(item.IsOk());
    GradientModelNode* node = static_cast<GradientModelNode*>(item.GetID());
    node->expanded = true;
}

void GradientDataViewCtrl::onCollapsed(wxDataViewEvent& event) {
    auto item = event.GetItem();
    wxASSERT(item.IsOk());
    GradientModelNode* node = static_cast<GradientModelNode*>(item.GetID());
    node->expanded = false;
}

}