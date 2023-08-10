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
private:
    std::shared_ptr<core::Flame> flame;
    int editingId;

    void onFileOpen(wxCommandEvent& event);
    void onExit(wxCommandEvent& event);
    void onAbout(wxCommandEvent& event);
    void onEnterFlameUpdate(wxCommandEvent& event);
    void onFocusFlameEdit(wxFocusEvent& event);
    void onFlameUpdate(wxCommandEvent& event);

    bool flameTextEqual(int textCtrlId);
};

wxDECLARE_EVENT(FLAME_UPDATE_EVENT, wxCommandEvent);

}