#pragma once

#include "flame_document.hpp"

class FlameView: public wxView {
public:
    FlameView();
    bool OnCreate(wxDocument *doc, long flags) override;
    void OnDraw(wxDC *dc) override;
    bool OnClose(bool deleteWindow = true) override;
    void OnChangeFilename() override;

    FlameDocument* getDocument() const;
    core::Flame* flame;

    wxDECLARE_DYNAMIC_CLASS(FlameView);
};