#pragma once

#include <map>
#include <memory>
#include <wx/dataview.h>
#include "../core/gradient.hpp"
#include "../core/preset_library.hpp"

namespace ui {

struct GradientModelNode {
    GradientModelNode(GradientModelNode* parent_): parent(parent_){ }
    GradientModelNode(): parent(NULL){ }
    virtual ~GradientModelNode() = default;
    virtual bool isContainer() const = 0;
    bool expanded = false;
    GradientModelNode* parent;
};

struct GradientLeafNode : public GradientModelNode {
    GradientLeafNode(GradientModelNode* parent, core::Gradient* gradient_): GradientModelNode(parent), gradient(gradient_) { }
    virtual bool isContainer() const override { return false; }
    core::Gradient* gradient;
};

struct GradientContainerNode: public GradientModelNode {
    GradientContainerNode(GradientModelNode* parent, std::string name_): GradientModelNode(parent), name(name_) { }
    std::string name;
    std::vector<std::unique_ptr<GradientLeafNode>> children;
    virtual bool isContainer() const override { return true; }
    void addLeaf(core::Gradient* g) {
        children.emplace_back(std::make_unique<GradientLeafNode>(this, g));
    }
};

class GradientModel: public wxDataViewModel {
public:
    GradientModel(core::PresetLibrary* presetLibrary);
    bool IsContainer(const wxDataViewItem& item) const override;
    wxDataViewItem GetParent(const wxDataViewItem& item) const override;
    unsigned GetChildren(const wxDataViewItem& item, wxDataViewItemArray& children) const override;
    void GetValue(wxVariant& variant, const wxDataViewItem& item, unsigned col) const override;
    bool SetValue(const wxVariant& variant, const wxDataViewItem& item, unsigned col) override;
private:
    std::map<std::string, core::PresetLibrary*> mapping;
    std::vector<std::unique_ptr<GradientContainerNode>> folders;
};

class GradientDataViewCtrl: public wxDataViewCtrl {
public:
    GradientDataViewCtrl(wxWindow* parent);
    ~GradientDataViewCtrl();
private:
    void onExpanded(wxDataViewEvent& event);
    void onCollapsed(wxDataViewEvent& event);
};

}