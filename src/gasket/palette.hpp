#pragma once

#include <boost/gil.hpp>
#include <memory>

namespace gasket {

class Palette {
public:
    Palette();
    Palette(boost::gil::rgb8_pixel_t color);
    Palette(boost::gil::rgb8_pixel_t color1, boost::gil::rgb8_pixel_t color2);
    std::string hexAt(int pos) const;
private:
    typedef unsigned char byte;
    static const unsigned PALETTE_WIDTH = 256;
    std::vector<byte> paletteData;
};

}