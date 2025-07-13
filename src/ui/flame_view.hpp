#pragma once

#include "content.hpp"
#include "flame_document.hpp"
#include <boost/signals2.hpp>

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

    boost::signals2::signal<void (XformTriangleContent)> triangleContentChanged;

private:
    AevusFrame* aevusFrame;
    FlameDocument* document;
    void sendTriangleContent();

    wxDECLARE_DYNAMIC_CLASS(FlameView);
};

}