#pragma once

#include "wxfb/code/wxfb_frame.h"
#include <wx/wx.h>

enum
{
    ID_Hello = 1
};

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
    virtual void onHello(wxCommandEvent& event);
    virtual void onExit(wxCommandEvent& event);
    virtual void onAbout(wxCommandEvent& event);
};

}