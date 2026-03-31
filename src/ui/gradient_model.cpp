#include "gradient_model.hpp"
#include <wx/bmpbndl.h>
#include <wx/mstream.h>
#include <wx/artprov.h>
#include <wx/dataview.h>
#include <wx/variant.h>
#include <wx/debug.h>
#include <wx/log.h>
#include <lunasvg.h>


namespace ui {

GradientModel::GradientModel(core::PresetLibrary* presetLibrary, wxDataViewColumn* bitmapColumn_):
    bitmapColumn(bitmapColumn_)
{
    mapping["Presets"] = presetLibrary;
    for (auto it=mapping.begin(); it != mapping.end(); it++) {
        folders.emplace_back(std::make_unique<GradientContainerNode>(nullptr, it->first));
        for (int i=0; i<it->second->gradients.size(); i++) {
            folders.back()->addLeaf(&it->second->gradients[i]);
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
    } else if (auto leaf = dynamic_cast<GradientLeafNode*>(node)) {
        switch (col) {
            case 0: variant << wxDataViewIconText(leaf->gradient->title); break;
            case 1: {
                auto imageWidth = std::max(bitmapColumn->GetWidth()-20, 256);
                auto it = nodeImage.find(leaf);
                if (it == nodeImage.end() || it->second.GetSize().GetWidth() != imageWidth) {
                    std::vector<uint8_t> pngBytes;
                    leaf->gradient->renderPNG(imageWidth, 20, pngBytes);
                    auto bitmap =  wxBitmap::NewFromPNGData(pngBytes.data(), pngBytes.size());
                    const_cast<GradientModel*>(this)->nodeImage[leaf] = bitmap;
                    it = nodeImage.find(leaf);
                    wxASSERT(it != nodeImage.end());
                }
                variant << wxBitmapBundle(it->second);
                break;
            }
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
    } else if (auto leaf = dynamic_cast<GradientLeafNode*>(node)) {
        switch (col) {
            case 0: {
                wxDataViewIconText iconText;
                iconText << variant;
                leaf->gradient->title = iconText.GetText();
                return true;
            }
            case 1:
                // leaf->entity->value = variant.GetLong();
                return false;
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