#pragma once

#include <cstdio>
#include <cmath>
#include <utility>
#include <memory>
#include <vector>

#include <tinyxml2.h>

#include "transform.h"

struct Mobius {
    cx a, b, c, d;
    Mobius();
    Mobius(cx a, cx b, cx c, cx d);

    void print();
    cx apply(cx z);
    cx applyInf();
    Mobius inverse();
    Mobius diagonalize();
    Mobius compose(Mobius n);
    Mobius conjugate(Mobius s);
    std::pair<cx,cx> fixedPoints();
    std::shared_ptr<Transform> decompose();

    static Mobius scaling(cx a);
    static Mobius translation(cx b);
    static Mobius invertAndReflect();
    static Mobius fromPoints(cx p, cx q, cx r);
    static Mobius fromPointsToPoints(cx p1, cx q1, cx r1, cx p2, cx q2, cx r2);
};

struct Gasket {
    std::vector<Mobius> m;
    Gasket(double fu, double v, double fv);
    void writeXMLFile(std::string filename);
    tinyxml2::XMLDocument xmlDoc;
};
