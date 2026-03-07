#pragma once

#include <wx/cmdproc.h>

#include "flame_view.hpp"

namespace ui {

class XFormUpdateCommand: public wxCommand {
public:
    XFormUpdateCommand(FlameView* flameView, int xformIndex, 
        std::shared_ptr<core::XForm> oldXForm, std::shared_ptr<core::XForm> newXForm);
    virtual bool Do() override;
    virtual bool Undo() override;
private:
    FlameView* flameView;
    int xformIndex;
    std::shared_ptr<core::XForm> oldXform;
    std::shared_ptr<core::XForm> newXform;
};

}