#pragma once

#include <wx/cmdproc.h>

#include "flame_view.hpp"

namespace ui {

class XFormUpdateCommand: public wxCommand {
public:
    XFormUpdateCommand(FlameView* flameView, int xformIndex, core::XForm oldXForm, core::XForm newXForm);
    virtual bool Do() override;
    virtual bool Undo() override;
private:
    FlameView* flameView;
    int xformIndex;
    core::XForm oldXform, newXform;
};

class XFormAddCommand: public wxCommand {
public:
    XFormAddCommand(FlameView* flameView, int xformIndex);
    virtual bool Do() override;
    virtual bool Undo() override;
private:
    FlameView* flameView;
    int xformIndex;
};

class XFormRemoveCommand: public wxCommand {
public:
    XFormRemoveCommand(FlameView* flameView, int xformIndex, core::XForm oldXForm);
    virtual bool Do() override;
    virtual bool Undo() override;
private:
    FlameView* flameView;
    int xformIndex;
    core::XForm oldXform;
};

class FrameUpdateCommand: public wxCommand {
public:
    FrameUpdateCommand(FlameView*, core::FrameContent oldFrame, core::FrameContent newFrame);
    virtual bool Do() override;
    virtual bool Undo() override;
private:
    FlameView* flameView;
    core::FrameContent oldFrame, newFrame;
};

}