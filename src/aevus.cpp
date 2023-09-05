#include <optional>
#include <wx/app.h>
#include "clwrap/opencl.hpp"
#include "ui/aevus_frame.hpp"

using std::optional;
using std::string;

class Aevus: public wxApp {
public:
    bool OnInit() override;
};

bool Aevus::OnInit() {
    wxImage::AddHandler(new wxPNGHandler());
    auto filename = argc < 2 ? (optional<string>)std::nullopt : argv[1].ToStdString();
    ui::AevusFrame* frame = new ui::AevusFrame(filename);
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
