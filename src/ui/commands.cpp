
#include "commands.hpp"

using core::Flame;
using core::XForm;

XFormUpdateCommand::XFormUpdateCommand(Flame* flame_, int xformIndex_,
    std::shared_ptr<XForm> oldXform_, std::shared_ptr<XForm> newXForm_): wxCommand(true, "Update XForm"),
    flame(flame_), xformIndex(xformIndex_), oldXform(oldXform_), newXform(newXForm_) { }

bool XFormUpdateCommand::Do() {
    flame->xforms.set(xformIndex, newXform);
    return true;
}

bool XFormUpdateCommand::Undo() {
    printf("got here\n");
    flame->xforms.set(xformIndex, oldXform);
    return true;
}

