#include <bits/types/clock_t.h>
#include <cstddef>
#include <gmpxx.h>
#include <ctime>
#include <memory>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <boost/gil.hpp>
#include <tinyxml2.h>
#include "zoom.hpp"
#include "complex_type.hpp"
#include "mobius.hpp"
#include "palette.hpp"
#include "scaler.hpp"
#include "sdf.hpp"
#include "diver.hpp"

void convertFlame(std::string source, std::string dest);

int main(int argc, char* argv[]) {
    auto palette = std::make_shared<Palette>(boost::gil::rgb8_pixel_t(255,255,255),
        boost::gil::rgb8_pixel_t(255,0,0));
    try {
        Shape<mpq_class> shape(mpq_class(6,11),
            mpq_class(3,7),
            Complex<mpq_class>(mpq_class(1,1),mpq_class(0,1)));
        Diver<mpq_class> diver(200, 314159);
        Colorer colorer;
        mpq_class prec(1);
        for (int i=0; i<10; i++) {
            prec = prec / 10;
        }
        mpq_class iniLogscale = mpq_class(-50,150);
        mpq_class step = mpq_class(1,150);
        Scaler<mpq_class> scaler(iniLogscale, step, 22050, prec);
        typedef Zoom<mpq_class, Diver<mpq_class>, Colorer> GasketZoom;
        GasketZoom gz(shape, diver, scaler, colorer, mpq_class(16, 9));

        tinyxml2::XMLDocument xmlDoc;
        auto node = gz.getFlame(10).toXMLNode(xmlDoc);
        xmlDoc.InsertFirstChild(node);
        xmlDoc.SaveFile(stdout);

    } catch (std::exception& e) {
        printf("Error occured: %s\n",e.what());
    }
    return 0;
}