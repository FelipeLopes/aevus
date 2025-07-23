#include "aevus_frame.hpp"
#include <wx/gdicmn.h>
#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <wx/mstream.h>
#include <wx/wfstream.h>
#include "../render/iterator.hpp"
#include "flame_model.hpp"
#include "transform_model.hpp"
#include "triangle_model.hpp"

using namespace boost::signals2;
using boost::bind;
using clwrap::OpenCL;
using render::Renderer;
using std::optional;
using std::string;

namespace ui {

AevusFrame::AevusFrame(wxDocManager* manager, OpenCL* openCL, optional<string> filename):
    WxfbFrame(manager, NULL),
    context(openCL->createQueuedContext(0, 1)),
    renderer(context, flameStream),
    flameModel(flameWindow, flameStream),
    preTransformModel(preTransformDataViewCtrl, resetPreButton, true),
    postTransformModel(postTransformDataViewCtrl, resetPostButton, false),
    weightsModel(weightsDataViewCtrl, addXformButton, removeXformButton),
    variationModel(variationListCtrl, variationTextCtrl),
    colorModel(colorListCtrl, palettePanel),
    frameModel(frameListCtrl),
    triangleModel(trianglePanel)
{
    renderer.imageRendered.connect(bind(&FlameModel::update, &flameModel));

    addXformButton->SetBitmap(loadEmbeddedPNG(
        _binary_res_plus_default_png_start,
        _binary_res_plus_default_png_end
    ));
    addXformButton->SetBitmapSelected(loadEmbeddedPNG(
        _binary_res_plus_selected_png_start,
        _binary_res_plus_selected_png_end
    ));
    removeXformButton->SetBitmap(loadEmbeddedPNG(
        _binary_res_minus_default_png_start,
        _binary_res_minus_default_png_end
    ));
    removeXformButton->SetBitmapSelected(loadEmbeddedPNG(
        _binary_res_minus_selected_png_start,
        _binary_res_minus_selected_png_end
    ));

    trianglePanel->SetFocus();

    if (filename.has_value()) {
        loadFlame(filename.value());
    }
}

void AevusFrame::setupFlameView(FlameView *flameView) {
    core::Flame* ptr = NULL;
    if (flameView != NULL) {
        ptr = flameView->getFlame();
        flameView->triangleContentChanged.connect(
            bind(&TriangleModel::handleContent, &triangleModel, _1)
        );
        flameView->startNewRender.connect(
            bind(&Renderer::update, &renderer)
        );
        flameView->noTransformContent.connect(
            bind(&TransformModel::handleNoContent, &preTransformModel)
        );
        flameView->noTransformContent.connect(
            bind(&TransformModel::handleNoContent, &postTransformModel)
        );
        flameView->preTransformContent.connect(
            bind(&TransformModel::handleContent, &preTransformModel, _1)
        );
        flameView->postTransformContent.connect(
            bind(&TransformModel::handleContent, &postTransformModel, _1)
        );
        flameView->weightsContent.connect(
            bind(&WeightsModel::handleContent, &weightsModel, _1)
        );
        flameView->variationContent.connect(
            bind(&VariationModel::handleContent, &variationModel, _1)
        );
        flameView->colorContent.connect(
            bind(&ColorModel::handleContent, &colorModel, _1)
        );
        triangleModel.xformSelected.connect(
            bind(&FlameView::handleXFormSelected, flameView, _1)
        );
        triangleModel.xformCoefsChanged.connect(
            bind(&FlameView::handleTriangleCoefs, flameView, _1)
        );
        preTransformModel.contentChanged.connect(
            bind(&FlameView::handleCoefsPreListCtrl, flameView, _1)
        );
        postTransformModel.contentChanged.connect(
            bind(&FlameView::handleCoefsPostListCtrl, flameView, _1)
        );
        weightsModel.xformSelected.connect(
            bind(&FlameView::handleXFormSelected, flameView, _1)
        );
        weightsModel.xformAdded.connect(
            bind(&FlameView::handleXFormAdded, flameView)
        );
        weightsModel.xformRemoved.connect(
            bind(&FlameView::handleXFormRemoved, flameView)
        );
        weightsModel.weightsChanged.connect(
            bind(&FlameView::handleWeights, flameView, _1)
        );
        variationModel.variationAdded.connect(
            bind(&FlameView::handleVariationAdded, flameView, _1)
        );
        variationModel.variationData.connect(
            bind(&FlameView::handleVariationData, flameView, _1)
        );
        colorModel.colorContent.connect(
            bind(&FlameView::handleColorContent, flameView, _1)
        );
    }
    renderer.setFlame(ptr);
    frameModel.setFlame(ptr);
}

wxBitmap AevusFrame::loadEmbeddedPNG(char* start, char* end) {
    size_t sz = end - start;
    wxMemoryInputStream stream(start, sz);
    wxImage image(stream, wxBITMAP_TYPE_PNG);
    return wxBitmap(image);
}

void AevusFrame::onTransformValueChanged(wxDataViewEvent& event) {
    switch (event.GetId()) {
        case ID_PRE_DATAVIEW: preTransformModel.handleValueChangedEvent(event); break;
        case ID_POST_DATAVIEW: postTransformModel.handleValueChangedEvent(event); break;
    }
}

void AevusFrame::onTransformSelected(wxDataViewEvent& event) {
    weightsModel.handleSelectionEvent(event);
}

void AevusFrame::onWeightEdited(wxDataViewEvent& event) {
    weightsModel.handleValueChangedEvent(event);
}

void AevusFrame::onButtonClick(wxCommandEvent& event) {
    int eventId = event.GetId();
    switch (eventId) {
        case ID_PRE_RESET_BUTTON: preTransformModel.handleReset(); break;
        case ID_POST_RESET_BUTTON: postTransformModel.handleReset(); break;
        case ID_ADD_XFORM_BUTTON: weightsModel.handleAddXform(); break;
        case ID_REMOVE_XFORM_BUTTON: weightsModel.handleRemoveXform(); break;
    }
}

void AevusFrame::onVariationAddEnter(wxCommandEvent& event) {
    variationModel.handleVariationAdd();
}

void AevusFrame::onVariationValueChanged(wxDataViewEvent& event) {
    variationModel.handleValueChangedEvent(event);
}

void AevusFrame::onColorValueChanged(wxDataViewEvent& event) {
    colorModel.handleValueChangedEvent(event);
}

void AevusFrame::onFrameValueChanged(wxDataViewEvent& event) {
    frameModel.handleValueChangedEvent(event);
}

void AevusFrame::onPaint(wxPaintEvent& event) {
    switch (event.GetId()) {
        case ID_FLAME_WINDOW: flameModel.handlePaint(); break;
        case ID_PALETTE_PANEL: colorModel.handlePaint(); break;
        case ID_TRIANGLE_PANEL: triangleModel.handlePaint(); break;
    }
}

void AevusFrame::onResize(wxSizeEvent& event) {
    switch (event.GetId()) {
        case ID_TRIANGLE_PANEL: triangleModel.handleResize(event); break;
    }
}

void AevusFrame::onDataViewLostFocus(wxFocusEvent& event) {
    switch (event.GetId()) {
        case ID_PRE_DATAVIEW: preTransformModel.handleKillFocusEvent(event); break;
        case ID_POST_DATAVIEW: postTransformModel.handleKillFocusEvent(event); break;
    }
}

void AevusFrame::onMouseDown(wxMouseEvent& event) {
    switch (event.GetId()) {
        case ID_PALETTE_PANEL: colorModel.handleMouseDown(event); break;
        case ID_TRIANGLE_PANEL: triangleModel.handleMouseDown(event); break;
    }
}

void AevusFrame::onMouseUp(wxMouseEvent& event) {
    switch (event.GetId()) {
        case ID_PALETTE_PANEL: colorModel.handleMouseUp(event); break;
        case ID_TRIANGLE_PANEL: triangleModel.handleMouseUp(event); break;
    }
}

void AevusFrame::onMouseMove(wxMouseEvent& event) {
    switch (event.GetId()) {
        case ID_PALETTE_PANEL: colorModel.handleMouseMove(event); break;
        case ID_TRIANGLE_PANEL: triangleModel.handleMouseMove(event); break;
    }
}

void AevusFrame::onMouseWheel(wxMouseEvent& event) {
    switch (event.GetId()) {
        case ID_TRIANGLE_PANEL: triangleModel.handleMouseWheel(event); break;
        case ID_FLAME_WINDOW: flameModel.handleMouseWheel(event); break;
    }
}

void AevusFrame::onExit(wxCommandEvent& event) {
    Close(true);
}

void AevusFrame::onAbout(wxCommandEvent& event) {
    wxMessageBox("This is an early alpha of the Aevus fractal flame editor.",
        "About Aevus", wxOK | wxICON_INFORMATION);
}

void AevusFrame::loadFlame(std::string filename) {
    /*
    FILE* inputStream = fopen(filename.c_str(), "r");
    if (inputStream == NULL) {
        printf("Error on opening file: %s\n", filename.c_str());
        return;
    }
    flame.deserialize(inputStream);
    fclose(inputStream);
    eventBroker.flameLoaded();
    if (flame.xforms.size() > 0) {
        eventBroker.activeXformChanged(0);
    }*/
}

/*void AevusFrame::onFileOpen(wxCommandEvent& event) {
    wxFileDialog openFileDialog(this, "Open flame file", "", "",
        "Flame files (*.flame)|*.flame|XML files|*.xml",
        wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL) {
        return;
    }
    auto filename = openFileDialog.GetPath().ToStdString();
    loadFlame(filename);
}

void AevusFrame::onFileSaveAs(wxCommandEvent& event) {
    wxFileDialog saveFileDialog(this, "Save flame file", "", "",
        "XML files (*.xml)|*.xml", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (saveFileDialog.ShowModal() == wxID_CANCEL) {
        return;
    }
    auto filename = saveFileDialog.GetPath().ToStdString();
    FILE* outputStream = fopen(filename.c_str(), "w");
    if (outputStream == NULL) {
        printf("Error on opening file: %s\n", filename.c_str());
        return;
    }
    flame.serialize(outputStream);
    fclose(outputStream);
}*/

}