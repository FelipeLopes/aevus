#include "palette.h"
#include <memory>

using std::make_unique;
using boost::gil::interleaved_view;
using boost::gil::rgb8_pixel_t;

Palette::Palette() {
    paletteData = make_unique<rgb8_pixel_t[]>(PALETTE_WIDTH);
    paletteView = interleaved_view(PALETTE_WIDTH, 1,
        paletteData.get(), PALETTE_WIDTH*sizeof(rgb8_pixel_t));
    for (int i=0; i<PALETTE_WIDTH; i++) {
        paletteView(i,0) = rgb8_pixel_t(255,255,255);
    }
}