#include "renderer.hpp"
#include "iterator.hpp"
#include "tone_mapper.hpp"
#include <cmath>

using clwrap::CLQueuedContext;
using core::Flame;
using std::stringstream;
using std::vector;

namespace render {

Renderer::Renderer(CLQueuedContext& context_, Flame* flame_, stringstream& stream_):
    context(context_), flame(flame_), stream(stream_), iterator(context),
    toneMapper(context), flameModified(true), idle(true)
{
    renderFlame();
}

void Renderer::update() {
    lock.lock();
    flameModified = true;
    lock.unlock();
    renderFlame();
}

void Renderer::renderFlame() {
    lock.lock();
    if (idle) {
        flameModified = false;
        idle = false;
        iterator.extractParams(flame, iteratorParams);
        toneMapper.extractParams(flame, toneMapperParams);
        iteratorParams.threshold =
            ceil((exp(accumulationThreshold/toneMapperParams.a)-1)/toneMapperParams.b);
        extractRendererParams();
        iterator.runAsync(iteratorParams, [this] (auto hist) {
            if (!flameModified) {
                toneMapper.runAsync(toneMapperParams, hist, [this] (auto imgData) {
                    if (!flameModified) {
                        writePNMImage(*imgData);
                        lock.lock();
                        idle = true;
                        lock.unlock();
                        imageRendered();
                    } else {
                        lock.lock();
                        idle = true;
                        lock.unlock();
                        renderFlame();
                    }
                });
            } else {
                lock.lock();
                idle = true;
                lock.unlock();
                renderFlame();
            }
        });
    }
    lock.unlock();
}

void Renderer::writePNMImage(vector<float>& imgData) {
    stream.str("");
    stream.clear();
    stream << "P6\n" << rendererParams.width << " " <<
        rendererParams.height << "\n255\n";
    auto bg = rendererParams.background;
    for (int i=0; i<imgData.size()/4; i++) {
        float a = imgData[4*i+3];
        float r = imgData[4*i];
        float g = imgData[4*i+1];
        float b = imgData[4*i+2];
        if (a <= 1.0f) {
            r = r*a + bg.r*(1-a);
            g = g*a + bg.g*(1-a);
            b = b*a + bg.b*(1-a);
        }
        switch (rendererParams.saturationMode) {
            case CHANNEL: {
                a = std::min({a, 1/r, 1/g, 1/b});
                r*=a;
                g*=a;
                b*=a;
                break;
            }
            case WHITE: {
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

void Renderer::extractRendererParams() {
    auto sz = flame->size.value();
    rendererParams.width = sz.width;
    rendererParams.height = sz.height;
    rendererParams.background = flame->background.value().toColorCL();
    rendererParams.saturationMode = WHITE;
}

Renderer::~Renderer() {
    context.callbackPool.join();
}

}