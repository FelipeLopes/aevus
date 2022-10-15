#pragma once

#include <string>
#include <complex>
#include <array>
#include <map>
#include <vector>

#include <tinyxml2.h>

typedef std::complex<double> cx;

struct Point {
    double x, y;
    Point();
    Point(double x_, double y_);
    Point(cx z);
};

struct Affine {
    Affine();
    Affine(double xx, double xy, double yx, double yy, double ox, double oy);
    Point o, x, y;
    Point apply(Point p);
    std::string coefString();
};

class XForm {
public:
    XForm();
    XForm(std::string variation, Affine pre, Affine post = Affine());
    double weight, color, opacity;
    std::map<std::string, double> variations;
    std::vector<double> chaos;
    std::string coefsString();
    std::string postString();
private:
    Affine pre, post;
    std::string affineString(Affine aff);
    void initParams();
};