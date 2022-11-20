#include <array>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <boost/assign.hpp>
#include <stdexcept>
#include "variation.hpp"
#include "xform.hpp"
#include "xform_cl.hpp"

namespace render {

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

XForm::XForm(Variation::VariationID variation, Affine pre_, Affine post_, double color_):
    color(color_), pre(pre_), post(post_) {

    weight = 0.5;
    opacity = 1;
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

XFormCL XForm::toXFormCL() {
    XFormCL xf;

    xf.a = pre.x.x;
    xf.b = pre.y.x;
    xf.c = pre.o.x;
    xf.d = pre.x.y;
    xf.e = pre.y.y;
    xf.f = pre.o.y;

    xf.pa = post.x.x;
    xf.pb = post.y.x;
    xf.pc = post.o.x;
    xf.pd = post.x.y;
    xf.pe = post.y.y;
    xf.pf = post.o.y;

    int sz = 0;
    for (auto k: variations) {
        xf.varData[sz].id = k.first;
        xf.varData[sz].weight = k.second;
        sz++;
    }
    xf.varData[sz].id = Variation::NO_VAR;

    return xf;
}

tinyxml2::XMLNode* XForm::toXMLNode(tinyxml2::XMLDocument &xmlDoc) {
    auto xform = xmlDoc.NewElement("xform");
    xform->SetAttribute("weight", weight);
    xform->SetAttribute("color", color);
    for (auto& var: variations) {
        auto it = Variation::names.find(var.first);
        if (it == Variation::names.end()) {
            throw std::runtime_error("Unknown variation ID");
        }
        xform->SetAttribute(it->second.c_str(), var.second);
    }
    xform->SetAttribute("coefs", coefsString().c_str());
    xform->SetAttribute("post", postString().c_str());
    xform->SetAttribute("chaos", chaosString().c_str());
    return xform;
}

}