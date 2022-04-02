#include <iostream>
#include <sstream>

#include "transform.h"

Point::Point() {
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
    std::stringstream buffer;
    buffer<<pre.x.x<<" "<<pre.x.y<<" "<<pre.y.x<<" "<<pre.y.y<<" "<<pre.o.x<<" "<<pre.o.y;
    xform->SetAttribute("coefs", buffer.str().c_str());
    node->InsertEndChild(xform);
}

Point Linear::apply(Point p) {
    return pre.apply(p);
}
