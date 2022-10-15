#include <gmpxx.h>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <tinyxml2.h>
#include "gasket.h"
#include "complex_type.h"
#include "mobius.h"

int main() {
    try {
        Gasket<mpq_class> g(mpq_class(6,11), mpq_class(3,7), Complex<mpq_class>(1));
        tinyxml2::XMLDocument xmlDoc;
        //auto node = g.toFlame().toXMLNode(xmlDoc);
        mpq_class prec(1);
        for (int i=0; i<8; i++) {
            prec = prec / 10;
        }
        auto sc = exp<mpq_class>(mpq_class(1), prec);
        g.setScale(exp<mpq_class>(mpq_class(1), prec));
        g.selectZoomPoint(314159, 200);
        g.toFlame();
        //xmlDoc.InsertFirstChild(node);
        //xmlDoc.SaveFile(stdout);
    } catch (std::exception& e) {
        printf("Error occured: %s\n",e.what());
    }
    return 0;
}