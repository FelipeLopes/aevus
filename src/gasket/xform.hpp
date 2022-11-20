#pragma once

#include <string>
#include <complex>
#include <array>
#include <map>
#include <vector>
#include <tinyxml2.h>

namespace gasket {

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
    enum VariationID {
        LINEAR = 0,
        SPHERICAL = 2
    };
    XForm();
    XForm(VariationID variation, Affine pre, Affine post = Affine(), double color = 0);
    double weight, color, opacity;
    std::map<VariationID, double> variations;
    std::vector<double> chaos;
    std::string coefsString();
    std::string postString();
    std::string chaosString();
    tinyxml2::XMLNode* toXMLNode(tinyxml2::XMLDocument& xmlDoc);
    static std::map<VariationID, std::string> variationName;
private:
    Affine pre, post;
    std::string affineString(Affine aff);
    void initParams();
};

}