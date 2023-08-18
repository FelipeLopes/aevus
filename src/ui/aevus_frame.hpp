#pragma once

#include <set>
#include <wx/wx.h>
#include <wx/textcompleter.h>
#include "transform_model.hpp"
#include "wxfb/code/wxfb_frame.h"
#include "../core/flame.hpp"

namespace ui {

class Aevus: public wxApp {
public:
    virtual bool OnInit();
};

class AevusFrame: public WxfbFrame {
public:
    AevusFrame(std::shared_ptr<core::Flame> flame);
    ~AevusFrame();
    void loadFile(std::string filename);
private:
    std::shared_ptr<core::Flame> flame;
    std::shared_ptr<TransformModel> preTransformModel;
    std::shared_ptr<TransformModel> postTransformModel;
    int editingId;
    int editingTransform;

    void onFileOpen(wxCommandEvent& event) override;
    void onFileSaveAs(wxCommandEvent& event) override;
    void onExit(wxCommandEvent& event) override;
    void onAbout(wxCommandEvent& event) override;
    void onResetFlameUpdate(wxCommandEvent& event) override;
    void onTransformChosen(wxCommandEvent& event) override;
    void onVariationAddEnter(wxCommandEvent& event) override;
    void onVariationValueChanged(wxDataViewEvent& event) override;
    void onTransformValueChanged(wxDataViewEvent& event) override;

    void onFlameUpdate(wxCommandEvent& event);
    void onFlameXformChange(wxCommandEvent& event);

    void fireFlameUpdateEvent();
    void fireFlameXformChangeEvent();
};

wxDECLARE_EVENT(FLAME_UPDATE_EVENT, wxCommandEvent);
wxDECLARE_EVENT(FLAME_XFORM_CHANGE_EVENT, wxCommandEvent);

class VariationTextCompleter: public wxTextCompleterSimple {
public:
    VariationTextCompleter();
    void GetCompletions(const wxString& prefix, wxArrayString& res) override;
private:
    std::set<std::string> validStrings;
};

}