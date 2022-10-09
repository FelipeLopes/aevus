#include <iostream>
#include <gmpxx.h>

#include "complex_type.h"
#include "mobius.h"
#include "gasket.h"

using namespace std;

const double EPS = 1e-10;
const double PI = 2*acos(0);

Mobius_::Mobius_() {
}

Mobius_::Mobius_(cx a_, cx b_, cx c_, cx d_) {
    cx sdet = sqrt(a_*d_ - b_*c_);
    a = a_/sdet;
    b = b_/sdet;
    c = c_/sdet;
    d = d_/sdet;
}

cx Mobius_::apply(cx z) {
    return (a*z+b)/(c*z+d);
}

cx Mobius_::applyInf() {
    return a/c;
}

void Mobius_::print() {
    printf("(%lf, %lf) (%lf, %lf)\n(%lf, %lf) (%lf, %lf)\n\n",a.real(),a.imag(),b.real(),b.imag(),
            c.real(),c.imag(),d.real(),d.imag());
}

void printCx(cx z) {
    printf("(%lf, %lf)\n",z.real(),z.imag());
}

Mobius_ Mobius_::scaling(cx a) {
    return Mobius_(a, 0, 0, 1);
}

Mobius_ Mobius_::translation(cx b) {
    return Mobius_(1, b, 0, 1);
}

Mobius_ Mobius_::invertAndReflect() {
    return Mobius_(0, 1, 1, 0);
} 

// Takes point p -> 0, q -> 1, r -> infty
Mobius_ Mobius_::fromPoints(cx p, cx q, cx r) {
    return Mobius_(q-r,-p*(q-r),(q-p),-r*(q-p));
}

// Inverts the transformation
Mobius_ Mobius_::inverse() {
    return Mobius_(d,-b,-c,a);
}

// Composes the transformation with n,
// returning the transformation given by applying n
// before the transformation
Mobius_ Mobius_::compose(Mobius_ n) {
    return Mobius_(a*n.a + b*n.c, a*n.b + b*n.d, c*n.a + d*n.c, c*n.b + d*n.d); 
}

// Returns a transformation that takes points
// p1 -> p2, q1 -> q2, r1 -> r2
Mobius_ Mobius_::fromPointsToPoints(cx p1, cx q1, cx r1, cx p2, cx q2, cx r2) {
    return fromPoints(p2, q2, r2).inverse().compose(fromPoints(p1, q1, r1));
}

Mobius_ Mobius_::conjugate(Mobius_ s) {
    return s.inverse().compose(*this).compose(s);
}

Mobius_ Mobius_::diagonalize() {
    cx trace = a + d;
    cx l = 0.5*(trace + sqrt(trace*trace-4.0));
    return Mobius_(b, l-d, trace-l-a, c);
}

pair<cx,cx> Mobius_::fixedPoints() {
    cx s = a - d;
    cx e = sqrt((a-d)*(a-d)+4.0*b*c);
    return make_pair((s+e)/(2.0*c),(s-e)/(2.0*c));
}

shared_ptr<Transform> Mobius_::decompose() {
    if (fabs(c) > EPS) {
        auto t = make_shared<Spherical>();

        Mobius_ preInv = Mobius_::translation(d/c);
        cx preOri = conj(preInv.apply(0));
        t->pre.o = Point(preOri);
        t->pre.x = Point(conj(preInv.apply(1))-preOri);
        t->pre.y = Point(conj(preInv.apply(1i))-preOri);

        Mobius_ postInv = Mobius_::translation(a/c).compose(Mobius_::scaling(-1.0/(c*c)));
        cx postOri = postInv.apply(0);
        t->post.o = Point(postOri);
        t->post.x = Point(postInv.apply(1)-postOri);
        t->post.y = Point(postInv.apply(1i)-postOri);

        return t;
    } else {
        auto t = make_shared<Linear>();

        cx ori = apply(0);
        t->pre.o = Point(ori);
        t->pre.x = Point(apply(1)-ori);
        t->pre.y = Point(apply(1i)-ori);

        return t;
    }
}

void print(Complex<mpq_class> c) {
    printf("(%s/%s)+(%s/%s)*i\n",
        c.real.get_num().get_str().c_str(),
        c.real.get_den().get_str().c_str(),
        c.imag.get_num().get_str().c_str(),
        c.imag.get_den().get_str().c_str());
}

template <typename T>
Gasket<T>::Gasket(T r1, T r2, Complex<T> f, bool flip) {

    if (r2 > r1) {
        throw std::invalid_argument("First radius parameter should be greater than "
            "or equal to the second.");
    } else if (r1 + r2 > 1) {
        throw std::invalid_argument("Radii sum cannot be larger than 1.");
    } else if (r1 < 0 || r2 < 0) {
        throw std::invalid_argument("All radii should be positive.");
    }

    if (f.norm() != 1) {
        throw std::invalid_argument("Phase must have unit norm.");
    }

    T a = -1;
    T b = 1/r1;
    T c = 1/r2;
    T s1 = a + b + c;
    T s2 = a*b + b*c + c*a;

    T e = 2*squareRoot<T>(s2);
    T d = s1 - e;
    if (d <= 0) {
        d =  s1 + e;
    }
    if (d < c) {
        throw std::invalid_argument("Radii given are not the largest circles.");
    }

    T l1 = r1 + r2;
    T l2 = 1 - r1;
    T l3 = 1 - r2;
    T cosx = (l2*l2+l3*l3-l1*l1)/(2*l2*l3);
    T sinx = squareRoot<T>(1-cosx*cosx);

    Complex<T> ii(T(0),T(1));
    Complex<T> p1 = f;
    Complex<T> p2 = p1*(Complex<T>(cosx)+Complex<T>(sinx)*ii);

    Complex<T> v1 = (Complex<T>(1)-Complex<T>(r1))*p1;
    Complex<T> v2 = (Complex<T>(1)-Complex<T>(r2))*p2;
    Complex<T> v3 = (r2*v1+r1*v2)/Complex<T>(r1+r2);

    auto m = Mobius<T>::fromPointsToPoints(
        Complex<T>(1),
        Complex<T>(-1),
        Complex<T>(0),
        p1,p2,v3);

    if (flip) {
        m = m.flip();
    }

    tr = Mobius<T>(Complex<T>(1),Complex<T>(0),
        Complex<T>(2)*ii,Complex<T>(1)).conjugate(m);
    rot = Mobius<T>::fromPointsToPoints(
        Complex<T>(0), Complex<T>(1), Complex<T>(-1),
        Complex<T>(1), Complex<T>(-1), Complex<T>(0)).conjugate(m);

    tr.normalize();
    rot.normalize();

    auto root = xmlDoc.NewElement("Flames");
    root->SetAttribute("name", "gasket");
    xmlDoc.InsertFirstChild(root);

    auto flame = xmlDoc.NewElement("flame");
    flame->SetAttribute("name", "Gasket");
    flame->SetAttribute("background", "0 0 0");
    flame->SetAttribute("version", "Apophysis 2.09");
    flame->SetAttribute("size", "600 600");
    flame->SetAttribute("center", "0 0");
    flame->SetAttribute("scale", "144");
    flame->SetAttribute("oversample", "1");
    flame->SetAttribute("filter", "0.2");
    flame->SetAttribute("quality", "1");
    flame->SetAttribute("background", "0 0 0");
    flame->SetAttribute("brightness", "4");
    flame->SetAttribute("gamma", "4");
    flame->SetAttribute("gamma_threshold", "0.04");
    root->InsertEndChild(flame);

    auto palette = xmlDoc.NewElement("palette");
    palette->SetAttribute("count", "256");
    palette->SetAttribute("format", "RGB");
    string whiteChars(48, 'F');
    string text = "\n";
    for (int i=0; i<32; i++) {
        text += (whiteChars + "\n");
    }
    palette->SetText(text.c_str());
    flame->InsertEndChild(palette);
}

template <typename T>
void Gasket<T>::writeXMLFile(string filename) {
    xmlDoc.SaveFile(filename.c_str());
}

int genGasket(int argc, char* argv[]) {
    ios::sync_with_stdio(false);

    if (argc < 7) {
        printf("Usage: %s [px] [py] [qx] [qy] [rx] [ry]\n", argv[0]);
        return 1;
    }

    cx p = cx(stod(argv[1]), stod(argv[2]));
    cx q = cx(stod(argv[3]), stod(argv[4]));
    cx r = cx(stod(argv[5]), stod(argv[6]));

    return 0;
}

template class Gasket<mpq_class>;