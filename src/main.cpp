#include "ui/aevus_frame.hpp"

wxIMPLEMENT_APP_NO_MAIN(ui::Aevus);

int main(int argc, char* argv[]) {
    wxEntryStart(argc, argv);
    wxTheApp->CallOnInit();
    wxTheApp->OnRun();
    return 0;
}
