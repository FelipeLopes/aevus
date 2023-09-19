#pragma once

#include "../clwrap/cl_context.hpp"
#include "iterator.hpp"
#include "tone_mapper.hpp"
#include <boost/signals2.hpp>

namespace render {

struct RendererParams {
    int width, height;
    core::ColorCL background;
};

class Renderer {
public:
    Renderer(clwrap::CLQueuedContext& context, core::Flame* flame, std::stringstream& stream);
    void renderFlame();
    boost::signals2::signal<void ()> imageRendered;
private:
    void writePNMImage(std::vector<float>& imgData);
    void extractRendererParams();
    std::stringstream& stream;
    core::Flame* flame;
    Iterator iterator;
    IteratorParams iteratorParams;
    ToneMapper toneMapper;
    ToneMapperParams toneMapperParams;
    RendererParams rendererParams;
};

}