#pragma once

#include "content.hpp"

namespace ui {

class TriangleHandler {
public:
    TriangleHandler();
    void handleContent(std::optional<XFormTriangleContent> content);
    void updateCoefsContent(int pos, CoefsContent coefsContent);
    void insertCoefsContent(int pos, CoefsContent coefsContent);
    void removeCoefsContent(int pos);
protected:
    std::optional<XFormTriangleContent> content;
};

}