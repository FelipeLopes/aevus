#pragma once

#include "../clwrap/cl_context.hpp"
#include "colorer.hpp"
#include "iterator.hpp"
#include "tone_mapper.hpp"
#include <boost/signals2.hpp>

namespace render {

enum RendererState {
    FLAME_MODIFIED,
    ITERATION_RUNNING,
    ITERATION_COMPLETED,
    FLAME_RENDERED
};

struct RendererParams {
    int width, height;
    core::ColorCL background;
    core::ClippingMode clippingMode;
};

class Renderer {
public:
    Renderer(clwrap::CLQueuedContext& context, core::Flame* flame, std::stringstream& stream);
    void update();
    ~Renderer();
    boost::signals2::signal<void ()> imageRendered;
private:
    void writePNMImage(std::vector<float>& imgData);
    void extractParams();
    void runIteration();
    void render();
    void extractRendererParams();
    clwrap::CLQueuedContext& context;
    core::Flame* flame;
    std::stringstream& stream;
    Iterator iterator;
    IteratorParams iteratorParams;
    ToneMapper toneMapper;
    ToneMapperParams toneMapperParams;
    Colorer colorer;
    RendererParams rendererParams;
    std::atomic<bool> running;
    std::atomic<RendererState> state;
    std::vector<float> histogram, imageData;
    std::mutex lock;
    const double accumulationThreshold = 10;
};

}