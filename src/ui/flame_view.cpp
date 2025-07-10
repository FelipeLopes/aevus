#include "flame_view.hpp"
#include <wx/app.h>

wxIMPLEMENT_DYNAMIC_CLASS(FlameView, wxView);

FlameView::FlameView() {
    SetFrame(wxTheApp->GetTopWindow());
}

bool FlameView::OnCreate(wxDocument *doc, long flags) {
    printf("OnCreate called\n");
    return wxView::OnCreate(doc, flags);
}

void FlameView::OnDraw(wxDC *dc) {
    // Stub function
}

bool FlameView::OnClose(bool deleteWindow) {
    return wxView::OnClose(deleteWindow);
}

void FlameView::OnChangeFilename() {
    // Stub function
}

FlameDocument* FlameView::getDocument() const
{
    return wxStaticCast(wxView::GetDocument(), FlameDocument);
}