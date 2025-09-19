#pragma once

#include "../core/content.hpp"
#include "flame_document.hpp"
#include "variation_model.hpp"
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
    void handleXFormSelected(int i);
    void handleXFormUpdate(core::ActiveXFormUpdateContent content);
    void handleXFormAdded(int id);
    void handleXFormRemoved(int id);
    void handleFrameContent(core::FrameContent content);

    boost::signals2::signal<void (std::optional<core::FlameContent>)> flameContent;
    boost::signals2::signal<void (core::ActiveXFormContent)> activeXformContent;

private:
    AevusFrame* aevusFrame;
    FlameDocument* document;
    int activeXformId;
    void sendFlameContent();
    void sendTriangleContent();
    void sendPreTransformContent();
    void sendPostTransformContent();
    void sendWeightsContent();
    void sendVariationContent();
    void sendColorContent();
    void sendFrameContent();
    void sendSelectedXFormContent();
    void sendUpdatedXFormContent();
    void sendAddedXFormContent();
    void sendRemovedXFormContent(int id);

    std::optional<core::XFormContent> getXformContent(int idx);

    wxDECLARE_DYNAMIC_CLASS(FlameView);
};

}