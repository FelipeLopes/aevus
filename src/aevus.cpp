#include <wx/app.h>
#include "clwrap/opencl.hpp"
#include "ui/aevus_frame.hpp"

class Aevus: public wxApp {
public:
    bool OnInit() override;
};

bool Aevus::OnInit()
{
    wxImage::AddHandler(new wxPNGHandler());
    auto flame = std::make_shared<core::Flame>();
    ui::AevusFrame* frame = new ui::AevusFrame(flame);
    frame->Show();
    return true;
}

wxIMPLEMENT_APP_NO_MAIN(Aevus);

int main(int argc, char* argv[]) {
    auto context = clwrap::OpenCL::getInstance().createQueuedContext(0, 1);
    wxEntryStart(argc, argv);
    wxTheApp->CallOnInit();
    wxTheApp->OnRun();
    return 0;
}
