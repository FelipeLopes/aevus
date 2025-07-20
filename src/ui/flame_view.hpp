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
    void handleXFormSelected(int i);
    void handleXFormAdded();
    void handleXFormRemoved();
    void handleTriangleCoefs(CoefsContent coefs);
    void handleCoefsPreListCtrl(CoefsContent coefs);
    void handleCoefsPostListCtrl(CoefsContent coefs);
    void handleWeights(WeightsContent content);

    boost::signals2::signal<void ()> startNewRender;
    boost::signals2::signal<void (XFormTriangleContent)> triangleContentChanged;
    boost::signals2::signal<void ()> noTransformContent;
    boost::signals2::signal<void (CoefsContent)> preTransformContent;
    boost::signals2::signal<void (CoefsContent)> postTransformContent;
    boost::signals2::signal<void (WeightsContent)> weightsContent;

private:
    AevusFrame* aevusFrame;
    FlameDocument* document;
    int activeXformId;
    void sendTriangleContent();
    void sendPreTransformContent();
    void sendPostTransformContent();
    void sendWeightsContent();

    wxDECLARE_DYNAMIC_CLASS(FlameView);
};

}