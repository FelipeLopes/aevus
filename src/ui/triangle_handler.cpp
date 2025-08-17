#include "triangle_handler.hpp"

namespace ui {

TriangleHandler::TriangleHandler() { }

void TriangleHandler::handleContent(std::optional<XFormTriangleContent> content) {
    this->content = content;
}

void TriangleHandler::updateCoefsContent(int pos, CoefsContent coefsContent) {
    if (!content.has_value()) {
        return;
    }
    content->activeId = pos;
    content->coefs[pos] = coefsContent;
}

}