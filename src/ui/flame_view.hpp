#pragma once

#include "content.hpp"
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
    void handleXFormAdded();
    void handleXFormRemoved();
    void handleTriangleCoefs(CoefsContent coefs);
    void handleCoefsPreListCtrl(CoefsContent coefs);
    void handleCoefsPostListCtrl(CoefsContent coefs);
    void handleWeights(WeightsContent content);
    void handleVariationAdded(core::Variation::VariationID id);
    void handleVariationData(VariationDataParams params);

    boost::signals2::signal<void ()> startNewRender;
    boost::signals2::signal<void (XFormTriangleContent)> triangleContentChanged;
    boost::signals2::signal<void ()> noTransformContent;
    boost::signals2::signal<void (CoefsContent)> preTransformContent;
    boost::signals2::signal<void (CoefsContent)> postTransformContent;
    boost::signals2::signal<void (WeightsContent)> weightsContent;
    boost::signals2::signal<void (VariationContent)> variationContent;

private:
    AevusFrame* aevusFrame;
    FlameDocument* document;
    int activeXformId;
    void sendTriangleContent();
    void sendPreTransformContent();
    void sendPostTransformContent();
    void sendWeightsContent();
    void sendVariationContent();

    wxDECLARE_DYNAMIC_CLASS(FlameView);
};

}