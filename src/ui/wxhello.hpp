#pragma once

#include "wxfb/code/aevus_frame.h"
#include <wx/wx.h>

enum
{
    ID_Hello = 1
};

namespace ui {

class MyApp: public wxApp
{
public:
    virtual bool OnInit();
};

}