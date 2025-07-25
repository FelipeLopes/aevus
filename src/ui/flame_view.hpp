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
    void handleColorContent(ColorContent content);
    void handleFrameContent(FrameContent content);

    boost::signals2::signal<void ()> startNewRender;
    boost::signals2::signal<void (std::optional<FlameContent>)> flameContent;
    boost::signals2::signal<void (ActiveXFormContent)> activeXformContent;
    boost::signals2::signal<void (XFormTriangleContent)> triangleContentChanged;
    boost::signals2::signal<void (WeightsContent)> weightsContent;
    boost::signals2::signal<void (VariationContent)> variationContent;
    boost::signals2::signal<void (ColorContent)> colorContent;
    boost::signals2::signal<void (FrameContent)> frameContent;

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
    void sendActiveXFormContent();

    XFormContent getXformContent(int idx);

    wxDECLARE_DYNAMIC_CLASS(FlameView);
};

}