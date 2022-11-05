#include "palette.hpp"
#include <memory>
#include <tuple>

using boost::gil::get_color;
using boost::gil::red_t;
using boost::gil::green_t;
using boost::gil::blue_t;
using boost::gil::interleaved_view;
using boost::gil::rgb8_pixel_t;

Palette::Palette(): Palette(rgb8_pixel_t(255,255,255)) {

}

Palette::Palette(rgb8_pixel_t color): Palette(color, color) {

}

Palette::Palette(rgb8_pixel_t color1, rgb8_pixel_t color2) {
    paletteData.resize(PALETTE_WIDTH);
    paletteView = interleaved_view(PALETTE_WIDTH, 1,
        (rgb8_pixel_t*)(&paletteData[0]), PALETTE_WIDTH*sizeof(rgb8_pixel_t));

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

std::string Palette::hexAt(int pos) const {
    char s[7];
    rgb8_pixel_t pix = paletteView(pos, 0);
    byte r = get_color(pix, red_t());
    byte g = get_color(pix, green_t());
    byte b = get_color(pix, blue_t());
    s[0] = r/16 > 9 ? (r/16)+'A'-10 : (r/16)+'0';
    s[1] = r%16 > 9 ? (r%16)+'A'-10 : (r%16)+'0';
    s[2] = g/16 > 9 ? (g/16)+'A'-10 : (g/16)+'0';
    s[3] = g%16 > 9 ? (g%16)+'A'-10 : (g%16)+'0';
    s[4] = b/16 > 9 ? (b/16)+'A'-10 : (b/16)+'0';
    s[5] = b%16 > 9 ? (b%16)+'A'-10 : (b%16)+'0';
    s[6] = '\0';
    return std::string(s);
}