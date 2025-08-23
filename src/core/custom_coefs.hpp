#pragma once

#include <array>
#include <vector>
#include "../serial/serializable.hpp"

namespace core {

class SizeParams: public serial::StringSerializable {
public:
    SizeParams();
    SizeParams(int width, int height);
    int width, height;
    virtual std::optional<std::string> toString();
    virtual void fromString(std::optional<std::string> text);
};

class CenterParams: public serial::StringSerializable {
public:
    CenterParams();
    CenterParams(double x, double y);
    double x, y;
    virtual std::optional<std::string> toString();
    virtual void fromString(std::optional<std::string> text);
};

class Affine: public serial::StringSerializable {
public:
    Affine(bool serializeIdentity);
    virtual std::optional<std::string> toString();
    virtual void fromString(std::optional<std::string> text);
    double xx, xy, yx, yy, ox, oy;
private:
    bool serializeIdentity;
};

class CoefsAffine: public Affine {
public:
    CoefsAffine();
};

class PostAffine: public Affine {
public:
    PostAffine();
};

class Chaos: public serial::StringSerializable {
public:
    Chaos();
    virtual std::optional<std::string> toString();
    virtual void fromString(std::optional<std::string> text);
    double getXaos(int idx) const;
private:
    std::vector<double> chaos;
};

class ColorSpeed: public serial::StringMapSerializable {
public:
    ColorSpeed();
    virtual std::map<std::string, std::string> toStringMap();
    virtual void fromStringMap(std::map<std::string, std::string> stringMap);
    double colorSpeed;
};

}