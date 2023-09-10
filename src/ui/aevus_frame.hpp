#pragma once

#include <boost/signals2.hpp>
#include <boost/bind.hpp>
#include <wxfb_frame.h>
#include "color_model.hpp"
#include "event_broker.hpp"
#include "flame_model.hpp"
#include "frame_model.hpp"
#include "transform_model.hpp"
#include "triangle_model.hpp"
#include "variation_model.hpp"
#include "weights_model.hpp"
#include "../clwrap/opencl.hpp"
#include "../core/flame.hpp"
#include "../render/renderer.hpp"

extern char _binary_res_plus_default_png_start[];
extern char _binary_res_plus_default_png_end[];
extern char _binary_res_plus_selected_png_start[];
extern char _binary_res_plus_selected_png_end[];
extern char _binary_res_minus_default_png_start[];
extern char _binary_res_minus_default_png_end[];
extern char _binary_res_minus_selected_png_start[];
extern char _binary_res_minus_selected_png_end[];

namespace ui {

class AevusFrame: public WxfbFrame {
public:
    AevusFrame(clwrap::OpenCL* openCL, std::optional<std::string> filename);
private:
    core::Flame flame;
    clwrap::CLQueuedContext context;
    std::stringstream flameStream;
    render::Renderer renderer;
    EventBroker eventBroker;

    FlameModel flameModel;
    TransformModel preTransformModel;
    TransformModel postTransformModel;
    WeightsModel weightsModel;
    VariationModel variationModel;
    ColorModel colorModel;
    FrameModel frameModel;
    TriangleModel triangleModel;

    void onFileOpen(wxCommandEvent& event) override;
    void onFileSaveAs(wxCommandEvent& event) override;
    void onExit(wxCommandEvent& event) override;
    void onAbout(wxCommandEvent& event) override;
    void onButtonClick(wxCommandEvent& event) override;
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

    void loadFlame(std::string filename);
    wxBitmap loadEmbeddedPNG(char* start, char* end);
};

}