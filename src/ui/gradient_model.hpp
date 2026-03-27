#pragma once

#include <map>
#include <memory>
#include <wx/dataview.h>

namespace ui {

struct Entity {
    std::string title;
    int value;
};

struct GradientModelNode {
    GradientModelNode(GradientModelNode* parent_): parent(parent_){ }
    GradientModelNode(): parent(NULL){ }
    virtual ~GradientModelNode() = default;
    virtual bool isContainer() const = 0;
    GradientModelNode* parent;
};

struct GradientEntityNode : public GradientModelNode {
    GradientEntityNode(GradientModelNode* parent, Entity* entity_): GradientModelNode(parent), entity(entity_) { }
    virtual bool isContainer() const override { return false; }
    Entity* entity;
};

struct GradientContainerNode: public GradientModelNode {
    GradientContainerNode(GradientModelNode* parent, std::string name_): GradientModelNode(parent), name(name_) { }
    std::string name;
    std::vector<std::unique_ptr<GradientEntityNode>> children;
    virtual bool isContainer() const override { return true; }
    void addEntityChild(Entity* e) {
        children.emplace_back(std::make_unique<GradientEntityNode>(this, e));
    }
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
    std::map<std::string, std::vector<Entity>> mapping;
    std::vector<std::unique_ptr<GradientContainerNode>> folders;
};

}