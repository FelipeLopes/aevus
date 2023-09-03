#pragma once

#include <boost/signals2.hpp>
#include <boost/bind.hpp>
#include <wxfb_frame.h>
#include "color_model.hpp"
#include "event_broker.hpp"
#include "frame_model.hpp"
#include "transform_model.hpp"
#include "triangle_model.hpp"
#include "variation_model.hpp"
#include "weights_model.hpp"
#include "../core/flame.hpp"

extern char _binary_res_plus_default_png_start[];
extern char _binary_res_plus_default_png_end[];
extern char _binary_res_plus_selected_png_start[];
extern char _binary_res_plus_selected_png_end[];
extern char _binary_res_minus_default_png_start[];
extern char _binary_res_minus_default_png_end[];
extern char _binary_res_minus_selected_png_start[];
extern char _binary_res_minus_selected_png_end[];
extern char _binary_res_minus_disabled_png_start[];
extern char _binary_res_minus_disabled_png_end[];

namespace ui {

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

    wxBitmap loadEmbeddedPNG(char* start, char* end);
};

}