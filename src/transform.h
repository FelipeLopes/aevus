#pragma once

#include <complex>

#include <tinyxml2.h>

typedef std::complex<double> cx;

struct Point {
    double x, y;
    Point();
    Point(double x_, double y_);
    Point(cx z);
};

struct Affine {
    Point o, x, y;
    Point apply(Point p);
};

struct Transform {
    virtual void print() = 0;
    virtual Point apply(Point p) = 0;
    virtual void addTransformXML(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* node) = 0;
    virtual ~Transform() {}
};

struct Spherical : public Transform {
    Affine pre, post;
    void print();
    Point apply(Point p);
    void addTransformXML(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* node);
};

struct Linear : public Transform {
    Affine pre;
    void print();
    Point apply(Point p);
    void addTransformXML(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* node);
};
