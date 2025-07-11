#include <optional>
#include <wx/app.h>
#include <wx/docview.h>
#include <wx/image.h>
#include <wx/imagpnm.h>
#include <wx/wfstream.h>
#include "clwrap/opencl.hpp"
#include "ui/aevus_frame.hpp"
#include "ui/flame_document.hpp"
#include "ui/flame_view.hpp"

using std::optional;
using std::string;

class Aevus: public wxApp {
public:
    bool OnInit() override;
private:
    std::unique_ptr<wxDocManager> docManager;
    wxDocTemplate* docTemplate;
    std::unique_ptr<ui::AevusFrame> frame;
};

bool Aevus::OnInit() {
    wxImage::AddHandler(new wxPNGHandler());
    wxImage::AddHandler(new wxPNMHandler());
    SetAppDisplayName("Aevus");
    auto openCL = clwrap::OpenCL::getInstance();
    auto filename = argc < 2 ? (optional<string>)std::nullopt : argv[1].ToStdString();
    docManager.reset(new wxDocManager);
    docManager->SetMaxDocsOpen(1);
    docTemplate = new wxDocTemplate(docManager.get(), "Fractal Flame", "*.flame", "", "flame",
        "FlameDocument", "FlameView", CLASSINFO(ui::FlameDocument), CLASSINFO(ui::FlameView));
    frame.reset(new ui::AevusFrame(docManager.get(), openCL, filename));
    SetTopWindow(frame.get());
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
