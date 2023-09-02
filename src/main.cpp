#include "clwrap/opencl.hpp"
#include "ui/aevus_frame.hpp"

wxIMPLEMENT_APP_NO_MAIN(ui::Aevus);

int main(int argc, char* argv[]) {
    auto context = clwrap::OpenCL::getInstance().createQueuedContext(0, 1);
    wxEntryStart(argc, argv);
    wxTheApp->CallOnInit();
    wxTheApp->OnRun();
    return 0;
}
