#pragma once

#include "flame_document.hpp"

namespace ui {

class AevusFrame;

class FlameView: public wxView {
public:
    FlameView();
    bool OnCreate(wxDocument *doc, long flags) override;
    void OnDraw(wxDC *dc) override;
    bool OnClose(bool deleteWindow = true) override;
    void OnChangeFilename() override;
    core::Flame* getFlame() const;
    void documentLoaded();

private:
    AevusFrame* aevusFrame;
    FlameDocument* document;

    wxDECLARE_DYNAMIC_CLASS(FlameView);
};

}