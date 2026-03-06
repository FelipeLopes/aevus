#pragma once

#include <wx/cmdproc.h>

#include "../core/flame.hpp"

class XFormUpdateCommand: public wxCommand {
public:
    XFormUpdateCommand(core::Flame* flame, int xformIndex, 
        std::shared_ptr<core::XForm> oldXForm, std::shared_ptr<core::XForm> newXForm);
    virtual bool Do() override;
    virtual bool Undo() override;
private:
    core::Flame* flame;
    int xformIndex;
    std::shared_ptr<core::XForm> oldXform;
    std::shared_ptr<core::XForm> newXform;
};