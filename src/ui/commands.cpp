
#include "commands.hpp"

using core::XForm;
using std::shared_ptr;

namespace ui {

XFormUpdateCommand::XFormUpdateCommand(FlameView* flameView_, int xformIndex_,
    shared_ptr<XForm> oldXform_, shared_ptr<XForm> newXForm_): wxCommand(true),
    flameView(flameView_), xformIndex(xformIndex_), oldXform(oldXform_), newXform(newXForm_) { }

bool XFormUpdateCommand::Do() {
    flameView->getFlame()->xforms.set(xformIndex, newXform);
    flameView->sendUpdatedXFormContent();
    return true;
}

bool XFormUpdateCommand::Undo() {
    flameView->getFlame()->xforms.set(xformIndex, oldXform);
    flameView->sendUpdatedXFormContent();
    return true;
}

XFormAddCommand::XFormAddCommand(FlameView* flameView_, int xformIndex_): wxCommand(true),
    flameView(flameView_), xformIndex(xformIndex_) { }

bool XFormAddCommand::Do() {
    flameView->getFlame()->xforms.appendAt(xformIndex, std::make_shared<core::XForm>());
    flameView->setActiveXFormId(xformIndex);
    flameView->sendAddedXFormContent();
    return true;
}

bool XFormAddCommand::Undo() {
    flameView->getFlame()->xforms.remove(xformIndex);
    flameView->setActiveXFormId(xformIndex);
    flameView->sendRemovedXFormContent(xformIndex);
    return true;
}

XFormRemoveCommand::XFormRemoveCommand(FlameView* flameView_, int xformIndex_, shared_ptr<XForm> oldXform_): wxCommand(true),
    flameView(flameView_), xformIndex(xformIndex_), oldXform(oldXform_) { }

bool XFormRemoveCommand::Do() {
    flameView->getFlame()->xforms.remove(xformIndex);
    flameView->setActiveXFormId(xformIndex);
    flameView->sendRemovedXFormContent(xformIndex);
    return true;
}

bool XFormRemoveCommand::Undo() {
    flameView->getFlame()->xforms.appendAt(xformIndex, std::make_shared<core::XForm>());
    flameView->getFlame()->xforms.set(xformIndex, oldXform);
    flameView->setActiveXFormId(xformIndex);
    flameView->sendAddedXFormContent();
    return true;
}

}