#pragma once

#include "../core/content.hpp"
#include "flame_document.hpp"
#include <boost/signals2.hpp>
#include <wx/cmdproc.h>

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
    void handleStartXFormExplore();
    void handleStopXFormExplore();

    boost::signals2::signal<void (std::optional<core::FlameContent>)> flameContent;
    boost::signals2::signal<void (core::ActiveXFormContent)> activeXformContent;
    boost::signals2::signal<void (core::FrameContent)> frameContent;

    void sendUpdatedXFormContent();
    void sendAddedXFormContent();
    void sendRemovedXFormContent(int id);
    void setActiveXFormId(int id);
    void sendFlameContent();
    void modifyDocument();

private:
    AevusFrame* aevusFrame;
    FlameDocument* document;
    int activeXformId;
    std::optional<core::XForm> xFormBeforeExplore;

    void sendTriangleContent();
    void sendPreTransformContent();
    void sendPostTransformContent();
    void sendWeightsContent();
    void sendVariationContent();
    void sendColorContent();
    void sendFrameContent();
    void sendSelectedXFormContent();
    std::optional<core::XFormContent> getXformContent(int idx);

    std::vector<boost::signals2::connection> connections;

    wxDECLARE_DYNAMIC_CLASS(FlameView);
};

}