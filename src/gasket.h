#pragma once
#include <cstdio>
#include <cmath>
#include <complex>

typedef std::complex<double> cx;

struct Point {
    double x, y;
    Point(){}
    Point(double x_, double y_) {
        x = x_;
        y = y_;
    }
    Point(cx z){
        x = z.real();
        y = z.imag();
    }
};

struct Affine {
    Point o, x, y;
    Point apply(Point p) {
        return Point(o.x + p.x*x.x + p.y*y.x, o.y + p.x*x.y+p.y*y.y);
    }
};

struct Transform {
    virtual void print() = 0;
    virtual Point apply(Point p) = 0;
    virtual ~Transform() {}
};

struct Spherical : public Transform {
    Affine pre, post;
    void print();
    Point apply(Point p);
};

struct Linear : public Transform {
    Affine pre;
    void print();
    Point apply(Point p);
};

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
