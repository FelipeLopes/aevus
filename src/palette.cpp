#include "palette.h"
#include <memory>

using std::make_unique;
using boost::gil::get_color;
using boost::gil::red_t;
using boost::gil::green_t;
using boost::gil::blue_t;
using boost::gil::interleaved_view;
using boost::gil::rgb8_pixel_t;

Palette::Palette() {
    init();
    for (int i=0; i<PALETTE_WIDTH; i++) {
        paletteView(i,0) = rgb8_pixel_t(255,255,255);
    }
}

Palette::Palette(rgb8_pixel_t color1, rgb8_pixel_t color2) {
    init();

    double r1 = get_color(color1, red_t());
    double g1 = get_color(color1, green_t());
    double b1 = get_color(color1, blue_t());

    double r2 = get_color(color2, red_t());
    double g2 = get_color(color2, green_t());
    double b2 = get_color(color2, blue_t());

    for (int i=0; i<PALETTE_WIDTH; i++) {
        double f = i / (PALETTE_WIDTH - 1.0);
        double r = r1*(1-f) + f*r2;
        double g = g1*(1-f) + f*g2;
        double b = b1*(1-f) + f*b2;
        paletteView(i,0) = rgb8_pixel_t((byte)(r+0.5), (byte)(g+0.5), (byte)(b+0.5));
    }
}

void Palette::init() {
    paletteData = make_unique<rgb8_pixel_t[]>(PALETTE_WIDTH);
    paletteView = interleaved_view(PALETTE_WIDTH, 1,
        paletteData.get(), PALETTE_WIDTH*sizeof(rgb8_pixel_t));
}