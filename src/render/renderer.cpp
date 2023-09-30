#include "renderer.hpp"
#include "iterator.hpp"
#include "tone_mapper.hpp"
#include <boost/asio/post.hpp>
#include <thread>
#include <cmath>

using clwrap::CLQueuedContext;
using core::Flame;
using std::stringstream;
using std::vector;

namespace render {

typedef std::chrono::duration<int, std::ratio<1, 60>> frame_duration;

Renderer::Renderer(CLQueuedContext& context_, Flame* flame_, stringstream& stream_):
    context(context_), flame(flame_), stream(stream_), iterator(context),
    toneMapper(context), colorer(context), running(true), state(FLAME_MODIFIED)
{
    update();
    boost::asio::post(context.rendererPool, [this] {
        while (running) {
            auto sleep_time = std::chrono::steady_clock::now() + frame_duration(1);
            switch (state) {
                case FLAME_MODIFIED: runIteration(); break;
                case ITERATION_RUNNING: /* should never happen */ break;
                case ITERATION_COMPLETED: render(); break;
                case FLAME_RENDERED: break;
            }
            std::this_thread::sleep_until(sleep_time);
        }
    });
}

void Renderer::update() {
    lock.lock();
    state = FLAME_MODIFIED;
    extractParams();
    lock.unlock();
}

void Renderer::writePNMImage(vector<float>& imgData) {
    stream.str("");
    stream.clear();
    int width = rendererParams.width;
    int height = rendererParams.height;
    stream << "P6\n" << width << " " << height << "\n255\n";
    auto bg = rendererParams.background;
    for (int i=0; i<width*height; i++) {
        float a = imgData[4*i+3];
        float r = imgData[4*i];
        float g = imgData[4*i+1];
        float b = imgData[4*i+2];
        if (a <= 1.0f) {
            r = r*a + bg.r*(1-a);
            g = g*a + bg.g*(1-a);
            b = b*a + bg.b*(1-a);
        }
        switch (rendererParams.clippingMode) {
            case core::CHANNEL: {
                a = std::min({a, 1/r, 1/g, 1/b});
                r*=a;
                g*=a;
                b*=a;
                break;
            }
            case core::WHITE: {
                r = std::min(r*a, 1.0f);
                g = std::min(g*a, 1.0f);
                b = std::min(b*a, 1.0f);
                break;
            }
            default: break;
        }
        stream.put((uint8_t)(r*255));
        stream.put((uint8_t)(g*255));
        stream.put((uint8_t)(b*255));
    }
}

void Renderer::extractParams() {
    iterator.extractParams(flame, iteratorParams);
    toneMapper.extractParams(flame, toneMapperParams);
    iteratorParams.threshold =
        ceil((exp(accumulationThreshold/toneMapperParams.a)-1)/toneMapperParams.b);
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

void Renderer::render() {
    lock.lock();
    auto event = toneMapper.runAsync(toneMapperParams, histogram);
    lock.unlock();
    if (state != FLAME_MODIFIED) {
        toneMapper.read(event, imageData);
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
    auto sz = flame->size.value();
    rendererParams.width = sz.width;
    rendererParams.height = sz.height;
    rendererParams.background = flame->background.value().toColorCL();
    rendererParams.clippingMode = flame->clipping.value().mode;
}

Renderer::~Renderer() {
    running = false;
    context.rendererPool.join();
}

}