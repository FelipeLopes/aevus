#include <optional>
#include <wx-3.2/wx/image.h>
#include <wx-3.2/wx/imagpnm.h>
#include <wx-3.2/wx/wfstream.h>
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
    wxImage::AddHandler(new wxPNMHandler());
    auto openCL = clwrap::OpenCL::getInstance();
    auto filename = argc < 2 ? (optional<string>)std::nullopt : argv[1].ToStdString();
    ui::AevusFrame* frame = new ui::AevusFrame(openCL, filename);
    frame->Show();
    return true;
}

wxIMPLEMENT_APP_NO_MAIN(Aevus);

int main(int argc, char* argv[]) {
    wxEntryStart(argc, argv);
    wxTheApp->CallOnInit();
    wxTheApp->OnRun();
    return 0;
}
