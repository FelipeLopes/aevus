#pragma once

#include <cstdio>
#include <cmath>

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
    Mobius compose(Mobius n);
    Mobius conjugate(Mobius s);
    Transform* decompose();

    static Mobius scaling(cx a);
    static Mobius translation(cx b);
    static Mobius invertAndReflect();
    static Mobius fromPoints(cx p, cx q, cx r);
    static Mobius fromPointsToPoints(cx p1, cx q1, cx r1, cx p2, cx q2, cx r2);
};

struct Gasket {
    Mobius m[4];
    Gasket(cx p, cx q, cx r);
    void writeXMLFile(std::string filename);
    tinyxml2::XMLDocument xmlDoc;
};
