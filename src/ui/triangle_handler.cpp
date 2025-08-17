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

void TriangleHandler::insertCoefsContent(int pos, CoefsContent coefsContent) {
    if (!content.has_value()) {
        return;
    }
    content->activeId = pos;
    auto it = std::next(content->coefs.begin(), pos);
    content->coefs.insert(it, coefsContent);
}

void TriangleHandler::removeCoefsContent(int pos) {
    if (!content.has_value()) {
        return;
    }
    content->activeId = pos;
    auto it = std::next(content->coefs.begin(), pos);
    content->coefs.erase(it);
    if (content->activeId == content->coefs.size()) {
        content->activeId--;
    }
}

}