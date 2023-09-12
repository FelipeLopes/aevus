#pragma once

#include "../clwrap/cl_context.hpp"
#include "iterator.hpp"
#include "tone_mapper.hpp"

namespace render {

class Renderer {
public:
    Renderer(const clwrap::CLQueuedContext& context, std::stringstream& stream);
    void renderFlame(core::Flame* flame);
    std::stringstream& stream;
    core::Flame* flame;
    Iterator iterator;
    ToneMapper toneMapper;
};

}