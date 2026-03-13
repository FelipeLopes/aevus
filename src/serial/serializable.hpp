#pragma once

#include <map>
#include <string>
#include <optional>

namespace serial {

std::string formattedDouble(double x);

class StringSerializable {
public:
    virtual std::optional<std::string> toString() = 0;
    virtual void fromString(std::optional<std::string> text) = 0;
    virtual ~StringSerializable() { }
};

class StringMapSerializable {
public:
    virtual std::map<std::string, std::string> toStringMap() = 0;
    virtual void fromStringMap(std::map<std::string, std::string> stringMap) = 0;
    virtual ~StringMapSerializable() { }
};

}