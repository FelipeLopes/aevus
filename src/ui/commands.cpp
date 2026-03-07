
#include "commands.hpp"

using core::Flame;
using core::XForm;

namespace ui {

XFormUpdateCommand::XFormUpdateCommand(FlameView* flameView_, int xformIndex_,
    std::shared_ptr<XForm> oldXform_, std::shared_ptr<XForm> newXForm_): wxCommand(true, "Update XForm"),
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

}