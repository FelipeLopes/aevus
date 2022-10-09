#pragma once

#include <cstdio>
#include <cmath>
#include <utility>
#include <memory>
#include <vector>

#include <tinyxml2.h>

#include "complex_type.h"
#include "mobius.h"
#include "transform.h"

struct Mobius_ {
    cx a, b, c, d;
    Mobius_();
    Mobius_(cx a, cx b, cx c, cx d);

    void print();
    cx apply(cx z);
    cx applyInf();
    Mobius_ inverse();
    Mobius_ diagonalize();
    Mobius_ compose(Mobius_ n);
    Mobius_ conjugate(Mobius_ s);
    std::pair<cx,cx> fixedPoints();
    std::shared_ptr<Transform> decompose();

    static Mobius_ scaling(cx a);
    static Mobius_ translation(cx b);
    static Mobius_ invertAndReflect();
    static Mobius_ fromPoints(cx p, cx q, cx r);
    static Mobius_ fromPointsToPoints(cx p1, cx q1, cx r1, cx p2, cx q2, cx r2);
};

template <typename T>
class Gasket {
public:
    Gasket(T r1, T r2, Complex<T> f, bool flip = false);
    void writeXMLFile(std::string filename);
    tinyxml2::XMLDocument xmlDoc;
private:
    Mobius<T> tr, rot;
};
