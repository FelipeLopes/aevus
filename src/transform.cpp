#include <array>
#include <iostream>
#include <iomanip>
#include <sstream>

#include "transform.h"

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

void Spherical::print() {
    printf("spherical\n");
    printf("pre\n");
    printf("X: %lf %lf\n",pre.x.x,pre.x.y);
    printf("Y: %lf %lf\n",pre.y.x,pre.y.y);
    printf("O: %lf %lf\n",pre.o.x,pre.o.y);
    printf("post\n");
    printf("X: %lf %lf\n",post.x.x,post.x.y);
    printf("Y: %lf %lf\n",post.y.x,post.y.y);
    printf("O: %lf %lf\n",post.o.x,post.o.y);
    printf("\n");
}

void Spherical::addTransformXML(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* node) {
    auto xform = doc.NewElement("xform");
    xform->SetAttribute("weight", "0.5");
    xform->SetAttribute("color", "0");
    xform->SetAttribute("spherical", "1");
    xform->SetAttribute("coefs", pre.coefString().c_str());
    xform->SetAttribute("post", post.coefString().c_str());
    node->InsertEndChild(xform);
}

Point Spherical::apply(Point p) {
    Point q = pre.apply(p);
    double r2 = q.x*q.x + q.y*q.y;
    return post.apply(Point(q.x/r2,q.y/r2));
}

void Linear::print() {
    printf("linear\n");
    printf("pre\n");
    printf("X: %lf %lf\n",pre.x.x,pre.x.y);
    printf("Y: %lf %lf\n",pre.y.x,pre.y.y);
    printf("O: %lf %lf\n",pre.o.x,pre.o.y);
    printf("post\n");
    printf("X: 1.000000 0.000000\n");
    printf("Y: 0.000000 1.000000\n");
    printf("O: 0.000000 0.000000\n");
    printf("\n");
}

void Linear::addTransformXML(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* node) {
    auto xform = doc.NewElement("xform");
    xform->SetAttribute("weight", "0.5");
    xform->SetAttribute("color", "0");
    xform->SetAttribute("linear", "1");
    xform->SetAttribute("coefs", pre.coefString().c_str());
    node->InsertEndChild(xform);
}

Point Linear::apply(Point p) {
    return pre.apply(p);
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

std::string XForm::affineString(Affine aff) {
    std::stringstream buffer;
    buffer<<std::fixed<<std::setprecision(6)<<
        aff.x.x<<" "<<aff.x.y<<" "<<
        aff.y.x<<" "<<aff.y.y<<" "<<
        aff.o.x<<" "<<aff.o.y;
    return buffer.str();
}