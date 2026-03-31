
#include "commands.hpp"
#include <wx-3.2/wx/cmdproc.h>

using core::FrameContent;
using core::XForm;

namespace ui {

FlameCommandProcessor::FlameCommandProcessor(): wxCommandProcessor() { }

// HACK: we are overriding this function in order to track document modifications.
// We want to use the MarkAsSaved() and IsDirty() methods of the command processor
// to tell us when the document is modified. However, the right time to call IsDirty()
// is AFTER Do() and Undo() have been executed in the command. That's precisely when
// SetMenuStrings() executes. So we override it calling the parent method first, and
// execute our document modification hooks immediately after.
void FlameCommandProcessor::SetMenuStrings() {
    wxCommandProcessor::SetMenuStrings();
    flameModified();
}

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
    if (xformIndex == -1) {
        xformIndex = 0;
    }
    auto pos = flameView->getFlame()->xforms.begin() + xformIndex;
    flameView->getFlame()->xforms.insert(pos, XForm());
    flameView->setActiveXFormId(xformIndex);
    flameView->sendAddedXFormContent();
    return true;
}

bool XFormAddCommand::Undo() {
    flameView->sendRemovedXFormContent(xformIndex);
    auto pos = flameView->getFlame()->xforms.begin() + xformIndex;
    flameView->getFlame()->xforms.erase(pos);
    if (xformIndex >= flameView->getFlame()->xforms.size()) {
        xformIndex--;
    }
    flameView->setActiveXFormId(xformIndex);
    return true;
}

XFormRemoveCommand::XFormRemoveCommand(FlameView* flameView_, int xformIndex_, XForm oldXform_): wxCommand(true),
    flameView(flameView_), xformIndex(xformIndex_), oldXform(oldXform_) { }

bool XFormRemoveCommand::Do() {
    flameView->sendRemovedXFormContent(xformIndex);
    auto pos = flameView->getFlame()->xforms.begin() + xformIndex;
    flameView->getFlame()->xforms.erase(pos);
    if (xformIndex >= flameView->getFlame()->xforms.size()) {
        xformIndex--;
    }
    flameView->setActiveXFormId(xformIndex);
    return true;
}

bool XFormRemoveCommand::Undo() {
    if (xformIndex == -1) {
        xformIndex = 0;
    }
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