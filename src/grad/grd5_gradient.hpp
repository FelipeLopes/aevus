#pragma once

#include <cstdio>
#include <inttypes.h>
#include <string>
#include <vector>

namespace grad {

class Grd5Extremum {
public:
    int type;
    union {
        double d;
        uint32_t u;
    };
};

class Grd5Extrema {
public:
    std::vector<Grd5Extremum> extremumList;
};

class Grd5GradientStop { };

class Grd5OpacityStop: public Grd5GradientStop {

};

class Grd5ColorStop: public Grd5GradientStop {

};

class Grd5Gradient {
public:
    std::string title;
};

class Grd5SolidGradient: public Grd5Gradient {
public:
    double smoothness;
    std::vector<Grd5OpacityStop> opacityStops;
    std::vector<Grd5ColorStop> colorStops;
};

class Grd5NoiseGradient: public Grd5Gradient {
public:
    bool showTransparency, vectorColor;
    uint32_t seed, smoothness;
    int model;
    Grd5Extrema min, max;
};

class Grd5GradientList {
public:
    std::vector<Grd5Gradient> gradients;
};

class Grd5Stream {
public:
    Grd5Stream(const char* filename);
    Grd5GradientList readGradientList();
private:
    void raiseFileStreamError();
    uint16_t readUint16();
    FILE* file;
};

}