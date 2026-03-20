
#include "commands.hpp"
#include <wx-3.2/wx/cmdproc.h>

using core::FrameContent;
using core::XForm;

namespace ui {

XFormUpdateCommand::XFormUpdateCommand(FlameView* flameView_, int xformIndex_, XForm oldXform_, XForm newXForm_): wxCommand(true),
    flameView(flameView_), xformIndex(xformIndex_), oldXform(oldXform_), newXform(newXForm_) { }

bool XFormUpdateCommand::Do() {
    flameView->getFlame()->xforms[xformIndex]  = newXform;
    flameView->sendUpdatedXFormContent();
    return true;
}

bool XFormUpdateCommand::Undo() {
    flameView->getFlame()->xforms[xformIndex] = oldXform;
    flameView->sendUpdatedXFormContent();
    return true;
}

XFormAddCommand::XFormAddCommand(FlameView* flameView_, int xformIndex_): wxCommand(true),
    flameView(flameView_), xformIndex(xformIndex_) { }

bool XFormAddCommand::Do() {
    auto pos = flameView->getFlame()->xforms.begin() + xformIndex;
    flameView->getFlame()->xforms.insert(pos, XForm());
    flameView->setActiveXFormId(xformIndex);
    flameView->sendAddedXFormContent();
    return true;
}

bool XFormAddCommand::Undo() {
    auto pos = flameView->getFlame()->xforms.begin() + xformIndex;
    flameView->getFlame()->xforms.erase(pos);
    flameView->setActiveXFormId(xformIndex);
    flameView->sendRemovedXFormContent(xformIndex);
    return true;
}

XFormRemoveCommand::XFormRemoveCommand(FlameView* flameView_, int xformIndex_, XForm oldXform_): wxCommand(true),
    flameView(flameView_), xformIndex(xformIndex_), oldXform(oldXform_) { }

bool XFormRemoveCommand::Do() {
    auto pos = flameView->getFlame()->xforms.begin() + xformIndex;
    flameView->getFlame()->xforms.erase(pos);
    flameView->setActiveXFormId(xformIndex);
    flameView->sendRemovedXFormContent(xformIndex);
    return true;
}

bool XFormRemoveCommand::Undo() {
    auto pos = flameView->getFlame()->xforms.begin() + xformIndex;
    flameView->getFlame()->xforms.insert(pos, XForm());
    flameView->getFlame()->xforms[xformIndex] = oldXform;
    flameView->setActiveXFormId(xformIndex);
    flameView->sendAddedXFormContent();
    return true;
}

FrameUpdateCommand::FrameUpdateCommand(FlameView* flameView_, FrameContent oldFrame_, FrameContent newFrame_):
    wxCommand(true), flameView(flameView_), oldFrame(oldFrame_), newFrame(newFrame_) { }

bool FrameUpdateCommand::Do() {
    auto flame = flameView->getFlame();
    flame->size = newFrame.flameSize;
    flame->center = newFrame.flameCenter;
    flame->scale = newFrame.flameScale;
    flameView->sendFlameContent();
    return true;
}

bool FrameUpdateCommand::Undo() {
    auto flame = flameView->getFlame();
    flame->size = oldFrame.flameSize;
    flame->center = oldFrame.flameCenter;
    flame->scale = oldFrame.flameScale;
    flameView->sendFlameContent();
    return true;
}

}