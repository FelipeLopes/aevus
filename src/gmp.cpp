#include <gmpxx.h>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <tinyxml2.h>
#include "gasket.h"
#include "complex_type.h"
#include "mobius.h"
#include "sdf.h"

int main(int argc, char* argv[]) {
    try {
        if (argc < 16) {
            return 1;
        }
        int numR1 = atoi(argv[1]);
        int denR1 = atoi(argv[2]);
        int numR2 = atoi(argv[3]);
        int denR2 = atoi(argv[4]);
        int numFR = atoi(argv[5]);
        int denFR = atoi(argv[6]);
        int numFI = atoi(argv[7]);
        int denFI = atoi(argv[8]);
        int level = atoi(argv[9]);
        int numLogscale = atoi(argv[10]);
        int denLogscale = atoi(argv[11]);
        int seed = atoi(argv[12]);
        int depth = atoi(argv[13]);
        int numAR = atoi(argv[14]);
        int denAR = atoi(argv[15]);
        //Gasket<mpq_class> g(mpq_class(6,11), mpq_class(3,7), Complex<mpq_class>(1));
        Gasket<mpq_class> g(mpq_class(numR1,denR1),
            mpq_class(numR2,denR2),
            Complex<mpq_class>(mpq_class(numFR,denFR),mpq_class(numFI,denFI)));
        tinyxml2::XMLDocument xmlDoc;
        mpq_class prec(1);
        for (int i=0; i<level; i++) { //10
            prec = prec / 10;
        }
        auto sc = exp<mpq_class>(mpq_class(numLogscale, denLogscale), prec);
        g.setScale(sc);
        g.selectZoomPoint(seed, depth); //314159, 200
        g.adapt(mpq_class(numAR, denAR)); //16, 9
        auto node = g.toFlame().toXMLNode(xmlDoc);;
        xmlDoc.InsertFirstChild(node);
        xmlDoc.SaveFile(stdout);
    } catch (std::exception& e) {
        printf("Error occured: %s\n",e.what());
    }
    return 0;
}