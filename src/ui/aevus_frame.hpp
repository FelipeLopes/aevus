#pragma once

#include <wx/wx.h>
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
    std::vector<wxTextCtrl*> textCtrls;
    int editingId;
    int editingTransform;

    void onFileOpen(wxCommandEvent& event);
    void onFileSaveAs(wxCommandEvent& event);
    void onExit(wxCommandEvent& event);
    void onAbout(wxCommandEvent& event);
    void onEnterFlameUpdate(wxCommandEvent& event);
    void onResetFlameUpdate(wxCommandEvent& event);
    void onFocusFlameEdit(wxFocusEvent& event);
    void onTransformChosen(wxCommandEvent& event);
    void onFlameUpdate(wxCommandEvent& event);

    void fireFlameUpdateEvent();
    bool flameTextEqual(int textCtrlId);
    int getCoefIndexByTextCtrlId(int textCtrlId);
    bool tryChangeAndUpdate(int textCtrlId);
};

wxDECLARE_EVENT(FLAME_UPDATE_EVENT, wxCommandEvent);

}