#pragma once

#include <boost/gil.hpp>
#include <memory>

class Palette {
public:
    Palette();
private:
    static const unsigned PALETTE_WIDTH = 256;
    std::unique_ptr<boost::gil::rgb8_pixel_t[]> paletteData;
    boost::gil::rgb8_view_t paletteView;
};