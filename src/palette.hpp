#pragma once

#include <boost/gil.hpp>
#include <memory>

class Palette {
public:
    Palette(boost::gil::rgb8_pixel_t color1, boost::gil::rgb8_pixel_t color2);
    std::string hexAt(int pos) const;
private:
    void init();
    typedef unsigned char byte;
    static const unsigned PALETTE_WIDTH = 256;
    std::vector<boost::gil::rgb8_pixel_t> paletteData;
    boost::gil::rgb8_view_t paletteView;
};