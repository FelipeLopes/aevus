#pragma once

#include "../clwrap/cl_context.hpp"
#include "iterator.hpp"
#include "tone_mapper.hpp"
#include <boost/signals2.hpp>

namespace render {

enum SaturationMode {
    ALPHA,
    CHANNEL,
    WHITE
};

struct RendererParams {
    int width, height;
    core::ColorCL background;
    SaturationMode saturationMode;
};

class Renderer {
public:
    Renderer(clwrap::CLQueuedContext& context, core::Flame* flame, std::stringstream& stream);
    void update();
    void renderFlame();
    ~Renderer();
    boost::signals2::signal<void ()> imageRendered;
private:
    void writePNMImage(std::vector<float>& imgData);
    void extractRendererParams();
    clwrap::CLQueuedContext& context;
    core::Flame* flame;
    std::stringstream& stream;
    Iterator iterator;
    IteratorParams iteratorParams;
    ToneMapper toneMapper;
    ToneMapperParams toneMapperParams;
    RendererParams rendererParams;
    bool flameModified, idle;
    std::mutex lock;
    const double accumulationThreshold = 10;
};

}