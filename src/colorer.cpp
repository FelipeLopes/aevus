#include "colorer.hpp"
#include "palette.hpp"

using boost::gil::rgb8_pixel_t;

const rgb8_pixel_t Colorer::RED = rgb8_pixel_t(255,0,0);
const rgb8_pixel_t Colorer::WHITE = rgb8_pixel_t(255,255,255);

Colorer::Colorer(): palette(WHITE, RED) {

}