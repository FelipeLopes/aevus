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
#include "gasket.hpp"
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
        /*if (argc < 16) {
            if (std::string(argv[1]) == "--convert") {
                if (argc < 4) {
                    return 1;
                }
                convertFlame(std::string(argv[2]),std::string(argv[3]));
                return 0;
            }
            return 1;
        }*/
        std::shared_ptr<Shape<mpq_class>> shape =
            std::make_shared<Shape<mpq_class>>(mpq_class(6,11),
            mpq_class(3,7),
            Complex<mpq_class>(mpq_class(1,1),mpq_class(0,1)));
        std::shared_ptr<Diver<mpq_class>> diver =
            std::make_shared<RandomDiver<mpq_class>>(200, 314159);
        mpq_class prec(1);
        for (int i=0; i<10; i++) {
            prec = prec / 10;
        }
        mpq_class iniLogscale = mpq_class(-50,150);
        mpq_class step = mpq_class(1,150);
        std::shared_ptr<Scaler<mpq_class>> scaler =
            std::make_shared<Scaler<mpq_class>>(iniLogscale, step, 22050, prec);
        Gasket<mpq_class> g(shape, diver, scaler, mpq_class(16, 9));

        tinyxml2::XMLDocument xmlDoc;
        auto node = g.getFlame(10, palette).toXMLNode(xmlDoc);
        xmlDoc.InsertFirstChild(node);
        xmlDoc.SaveFile(stdout);

    } catch (std::exception& e) {
        printf("Error occured: %s\n",e.what());
    }
    return 0;
}