#pragma once

#include "flame_document.hpp"

class FlameView: public wxView {
public:
    bool OnCreate(wxDocument *doc, long flags) override;
    void OnDraw(wxDC *dc) override;
    bool OnClose(bool deleteWindow = true) override;
    void OnChangeFilename() override;

    FlameDocument* getDocument() const;

    wxDECLARE_DYNAMIC_CLASS(FlameView);
};