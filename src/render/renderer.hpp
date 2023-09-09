#pragma once

#include "../clwrap/cl_context.hpp"
#include "iterator.hpp"
#include "tone_mapper.hpp"

namespace render {

class Renderer {
public:
    Renderer(const clwrap::CLQueuedContext& context);
    std::stringstream stream;
private:
    core::Flame flame;
    std::vector<float> dummy;
    Iterator iterator;
    ToneMapper toneMapper;
};

}