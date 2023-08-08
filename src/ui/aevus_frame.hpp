#pragma once

#include "event_id.hpp"
#include "wxfb/code/wxfb_frame.h"
#include <wx/wx.h>

namespace ui {

class MyApp: public wxApp {
public:
    virtual bool OnInit();
};

class AevusFrame: public WxfbFrame {
public:
    AevusFrame(wxWindow* parent);
    ~AevusFrame();
protected:
    void onHello(wxCommandEvent& event);
    void onExit(wxCommandEvent& event);
    void onAbout(wxCommandEvent& event);
};

}