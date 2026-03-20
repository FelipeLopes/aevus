#pragma once

#include <optional>
#include <string>
#include <map>
#include <vector>

namespace core {

class SizeParams {
public:
    SizeParams();
    SizeParams(int width, int height);
    int width, height;
    std::optional<std::string> toString();
    void fromString(std::optional<std::string> text);
};

class CenterParams {
public:
    CenterParams();
    CenterParams(double x, double y);
    double x, y;
    std::optional<std::string> toString();
    void fromString(std::optional<std::string> text);
};

class Affine {
public:
    Affine(bool serializeIdentity);
    std::optional<std::string> toString();
    void fromString(std::optional<std::string> text);
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

class Chaos {
public:
    Chaos();
    std::optional<std::string> toString();
    void fromString(std::optional<std::string> text);
    double getXaos(int idx) const;
private:
    std::vector<double> chaos;
};

class ColorSpeed {
public:
    ColorSpeed();
    virtual std::map<std::string, std::string> toStringMap();
    virtual void fromStringMap(std::map<std::string, std::string> stringMap);
    double colorSpeed;
};

}