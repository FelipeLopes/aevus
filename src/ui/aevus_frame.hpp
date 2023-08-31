#pragma once

#include <wx/app.h>
#include <boost/signals2.hpp>
#include <boost/bind.hpp>
#include "color_model.hpp"
#include "event_broker.hpp"
#include "frame_model.hpp"
#include "transform_model.hpp"
#include "triangle_model.hpp"
#include "variation_model.hpp"
#include "weights_model.hpp"
#include "wxfb/code/wxfb_frame.h"
#include "../core/flame.hpp"

namespace ui {

class Aevus: public wxApp {
public:
    virtual bool OnInit();
};

class AevusFrame: public WxfbFrame {
public:
    AevusFrame(std::shared_ptr<core::Flame> flame);
    ~AevusFrame();
    void loadFile(std::string filename);
private:
    std::shared_ptr<core::Flame> flame;
    std::shared_ptr<EventBroker> eventBroker;

    std::shared_ptr<TransformModel> preTransformModel;
    std::shared_ptr<TransformModel> postTransformModel;
    std::shared_ptr<WeightsModel> weightsModel;
    std::shared_ptr<VariationModel> variationModel;
    std::shared_ptr<ColorModel> colorModel;
    std::shared_ptr<FrameModel> frameModel;
    std::shared_ptr<TriangleModel> triangleModel;

    void onFileOpen(wxCommandEvent& event) override;
    void onFileSaveAs(wxCommandEvent& event) override;
    void onExit(wxCommandEvent& event) override;
    void onAbout(wxCommandEvent& event) override;
    void onResetFlameUpdate(wxCommandEvent& event) override;
    void onVariationAddEnter(wxCommandEvent& event) override;
    void onVariationValueChanged(wxDataViewEvent& event) override;
    void onTransformValueChanged(wxDataViewEvent& event) override;
    void onTransformSelected(wxDataViewEvent& event) override;
    void onWeightEdited(wxDataViewEvent& event) override;
    void onColorValueChanged(wxDataViewEvent& event) override;
    void onFrameValueChanged(wxDataViewEvent& event) override;
    void onPaint(wxPaintEvent& event) override;
    void onResize(wxSizeEvent& event) override;
    void onDataViewLostFocus(wxFocusEvent& event) override;
    void onMouseDown(wxMouseEvent& event) override;
    void onMouseUp(wxMouseEvent& event) override;
    void onMouseMove(wxMouseEvent& event) override;
    void onMouseWheel(wxMouseEvent& event) override;
};

}