#pragma once

#include "content.hpp"

namespace ui {

class TriangleHandler {
public:
    TriangleHandler();
    void handleContent(std::optional<XFormTriangleContent> content);
    void updateCoefsContent(int pos, CoefsContent coefsContent);
protected:
    std::optional<XFormTriangleContent> content;
};

}