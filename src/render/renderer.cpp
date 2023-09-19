#include "renderer.hpp"
#include "iterator.hpp"
#include "tone_mapper.hpp"

using clwrap::CLQueuedContext;
using core::Flame;
using std::stringstream;
using std::vector;

namespace render {

Renderer::Renderer(CLQueuedContext& context, Flame* flame_, stringstream& stream_): stream(stream_),
    flame(flame_), iterator(context), toneMapper(context), flameModified(true), idle(true)
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
    float bg_ra = bg.r * bg.a;
    float bg_ga = bg.g * bg.a;
    float bg_ba = bg.b * bg.a;
    for (int i=0; i<imgData.size()/4; i++) {
        float a = imgData[4*i+3];
        a = std::min(a, 1.0f);
        float r = imgData[4*i]*a + bg_ra*(1-a);
        float g = imgData[4*i+1]*a + bg_ga*(1-a);
        float b = imgData[4*i+2]*a + bg_ba*(1-a);
        float af = a + bg.a - a*bg.a;

        r /= af;
        g /= af;
        b /= af;

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
}

}