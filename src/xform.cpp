#include <array>
#include <iostream>
#include <iomanip>
#include <sstream>

#include "xform.hpp"

Point::Point() {
    x = 0;
    y = 0;
}

Point::Point(double x_, double y_) {
    x = x_;
    y = y_;
}

Point::Point(cx z) {
    x = z.real();
    y = z.imag();
}

Point Affine::apply(Point p) {
    return Point(o.x + p.x*x.x + p.y*y.x, o.y + p.x*x.y+p.y*y.y);
}

std::string Affine::coefString() {
    std::stringstream buffer;
    buffer<<std::fixed<<std::setprecision(6)<<x.x<<" "<<x.y<<" "<<y.x<<" "<<y.y<<" "<<o.x<<" "<<o.y;
    return buffer.str();
}

Affine::Affine() {
    o = Point(0,0);
    x = Point(1,0);
    y = Point(0,1);
}

Affine::Affine(double xx, double xy, double yx, double yy, double ox, double oy) {
    o = Point(ox,oy);
    x = Point(xx,xy);
    y = Point(yx,yy);
}

XForm::XForm() {
    initParams();
}

XForm::XForm(std::string variation, Affine pre_, Affine post_):pre(pre_),post(post_) {
    initParams();
    variations[variation] = 1;
}

void XForm::initParams() {
    weight = 0.5;
    color = 0;
    opacity = 1;
}

std::string XForm::coefsString() {
    return affineString(pre);
}

std::string XForm::postString() {
    return affineString(post);
}

std::string XForm::chaosString() {
    std::stringstream buffer;
    buffer<<std::fixed<<std::setprecision(6);
    for (int i=0; i<chaos.size(); i++) {
        buffer<<chaos[i]<<" ";
    }
    return buffer.str();
}

std::string XForm::affineString(Affine aff) {
    std::stringstream buffer;
    buffer<<std::fixed<<std::setprecision(6)<<
        aff.x.x<<" "<<aff.x.y<<" "<<
        aff.y.x<<" "<<aff.y.y<<" "<<
        aff.o.x<<" "<<aff.o.y;
    return buffer.str();
}

tinyxml2::XMLNode* XForm::toXMLNode(tinyxml2::XMLDocument &xmlDoc) {
    auto xform = xmlDoc.NewElement("xform");
    xform->SetAttribute("weight", weight);
    xform->SetAttribute("color", color);
    for (auto var: variations) {
        xform->SetAttribute(var.first.c_str(), var.second);
    }
    xform->SetAttribute("coefs", coefsString().c_str());
    xform->SetAttribute("post", postString().c_str());
    xform->SetAttribute("chaos", chaosString().c_str());
    return xform;
}