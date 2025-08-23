#include "renderer.hpp"
#include "iterator.hpp"
#include "tone_mapper.hpp"
#include <boost/asio/post.hpp>
#include <thread>
#include <cmath>

using clwrap::CLQueuedContext;
using std::stringstream;
using std::vector;

namespace render {

typedef std::chrono::duration<int, std::ratio<1, 60>> frame_duration;

Renderer::Renderer(CLQueuedContext& context_, stringstream& stream_):
    context(context_), stream(stream_), iterator(context),
    toneMapper(context), colorer(context), running(true), state(NO_FLAME)
{
    boost::asio::post(context.rendererPool, [this] {
        while (running) {
            auto sleep_time = std::chrono::steady_clock::now() + frame_duration(1);
            switch (state) {
                case NO_FLAME: break;
                case FLAME_MODIFIED: runIteration(); break;
                case ITERATION_RUNNING: /* do nothing, already busy */ break;
                case ITERATION_COMPLETED: runToneMapping(); break;
                case MAPPING_COMPLETED: render(); break;
                case FLAME_RENDERED: break;
            }
            std::this_thread::sleep_until(sleep_time);
        }
    });
}

void Renderer::handleFlameContent(std::optional<core::FlameContent> content) {
    lock.lock();
    this->content = content;
    if (!content.has_value()) {
        state = NO_FLAME;
    } else {
        state = FLAME_MODIFIED;
        extractParams();
    }
    // TODO: this is called when the document is closed. Verify that it terminates gracefully.
    lock.unlock();
}

void Renderer::writePNMImage(vector<uint8_t>& imgData) {
    stream.str("");
    stream.clear();
    int width = rendererParams.width;
    int height = rendererParams.height;
    stream << "P6\n" << width << " " << height << "\n255\n";
    for (int i=0; i<3*width*height; i++) {
        stream.put(imageData[i]);
    }
}

void Renderer::extractParams() {
    iterator.extractParams(content.value(), iteratorParams);
    toneMapper.extractParams(content.value(), toneMapperParams);
    iteratorParams.threshold =
        ceil((exp(accumulationThreshold/toneMapperParams.a)-1)/toneMapperParams.b);
    colorer.extractParams(content.value(), colorerParams);
    extractRendererParams();
}

void Renderer::runIteration() {
    lock.lock();
    state = ITERATION_RUNNING;
    auto event = iterator.runAsync(iteratorParams);
    lock.unlock();
    if (state != FLAME_MODIFIED) {
        iterator.read(event, histogram);
        lock.lock();
        if (state != FLAME_MODIFIED) {
            state = ITERATION_COMPLETED;
        }
        lock.unlock();
    }
}

void Renderer::runToneMapping() {
    lock.lock();
    auto event = toneMapper.runAsync(toneMapperParams, histogram);
    lock.unlock();
    if (state != FLAME_MODIFIED) {
        toneMapper.read(event, density);
        lock.lock();
        if (state != FLAME_MODIFIED) {
            state = MAPPING_COMPLETED;
        }
        lock.unlock();
    }
}

void Renderer::render() {
    lock.lock();
    auto event = colorer.runAsync(colorerParams, density);
    lock.unlock();
    if (state != FLAME_MODIFIED) {
        colorer.read(event, imageData);
        writePNMImage(imageData);
        lock.lock();
        if (state != FLAME_MODIFIED) {
            state = FLAME_RENDERED;
            imageRendered();
        }
        lock.unlock();
    }
}

void Renderer::extractRendererParams() {
    auto sz = content->frame.flameSize;
    rendererParams.width = sz.width;
    rendererParams.height = sz.height;
    rendererParams.background = content->render.background.toColorCL();
    rendererParams.clippingMode = content->render.clipping;
}

Renderer::~Renderer() {
    running = false;
    context.rendererPool.join();
}

}