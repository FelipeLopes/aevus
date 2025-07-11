#pragma once

#include "../clwrap/cl_context.hpp"
#include "colorer.hpp"
#include "iterator.hpp"
#include "tone_mapper.hpp"
#include <boost/signals2.hpp>

namespace render {

enum RendererState {
    NO_FLAME,
    FLAME_MODIFIED,
    ITERATION_RUNNING,
    ITERATION_COMPLETED,
    MAPPING_COMPLETED,
    FLAME_RENDERED
};

struct RendererParams {
    int width, height;
    core::ColorCL background;
    core::ClippingMode clippingMode;
};

class Renderer {
public:
    Renderer(clwrap::CLQueuedContext& context, std::stringstream& stream);
    void setFlame(core::Flame* flame);
    void update();
    ~Renderer();
    boost::signals2::signal<void ()> imageRendered;
private:
    void writePNMImage(std::vector<uint8_t>& imgData);
    void extractParams();
    void runIteration();
    void runToneMapping();
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
    ColorerParams colorerParams;
    RendererParams rendererParams;
    std::atomic<bool> running;
    std::atomic<RendererState> state;
    std::vector<float> histogram, density;
    std::vector<uint8_t> imageData;
    std::mutex lock;
    const double accumulationThreshold = 10;
};

}