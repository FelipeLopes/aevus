#pragma once

#include "../core/content.hpp"

namespace ui {

class TriangleHandler {
public:
    TriangleHandler();
    void handleContent(std::optional<core::XFormTriangleContent> content);
    void updateCoefsContent(int pos, core::CoefsContent coefsContent);
    void insertCoefsContent(int pos, core::CoefsContent coefsContent);
    void removeCoefsContent(int pos);
protected:
    std::optional<core::XFormTriangleContent> content;
};

}