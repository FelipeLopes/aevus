#include "wxhello.hpp"
#include "wxfb/code/aevus_frame.h"

namespace ui {

bool MyApp::OnInit()
{
    AevusFrame* frame = new AevusFrame(NULL);
    frame->Show( true );
    return true;
}

}