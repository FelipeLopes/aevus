#include "flame_view.hpp"

wxIMPLEMENT_DYNAMIC_CLASS(FlameView, wxView);

bool FlameView::OnCreate(wxDocument *doc, long flags) {
    return true;
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