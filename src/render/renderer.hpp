#pragma once

#include "../clwrap/cl_context.hpp"
#include "iterator.hpp"
#include "tone_mapper.hpp"
#include <boost/signals2.hpp>

namespace render {

class Renderer {
public:
    Renderer(clwrap::CLQueuedContext& context, core::Flame* flame, std::stringstream& stream);
    void renderFlame();
    boost::signals2::signal<void ()> imageRendered;
    std::stringstream& stream;
    core::Flame* flame;
    Iterator iterator;
    ToneMapper toneMapper;
private:
    void writePNMImage(std::vector<float>& imgData, int width, int height, core::ColorCL background);
};

}