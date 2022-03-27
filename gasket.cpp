#include <cstdio>
#include <cmath>
#include <complex>

using namespace std;

typedef complex<double> cx;

const double EPS = 1e-10;

struct Point {
    double x, y;
    Point(){}
    Point(double x_, double y_) {
        x = x_;
        y = y_;
    }
    Point(cx z){
        x = z.real();
        y = z.imag();
    }
};

struct Affine {
    Point o, x, y;
    Point apply(Point p) {
        return Point(o.x + p.x*x.x + p.y*y.x, o.y + p.x*x.y+p.y*y.y);
    }
};

struct Transform {
    virtual void print() = 0;
    virtual Point apply(Point p) = 0;
};

struct Spherical : public Transform {
    Affine pre, post;
    void print();
    Point apply(Point p);
};

struct Linear : public Transform {
    Affine pre;
    void print();
    Point apply(Point p);
};

struct Mobius {
    cx a, b, c, d;
    Mobius();
    Mobius(cx a, cx b, cx c, cx d);

    void print();
    cx apply(cx z);
    cx applyInf();
    Mobius inverse();
    Mobius compose(Mobius n);
    Mobius conjugate(Mobius s);
    Transform* decompose();

    static Mobius scaling(cx a);
    static Mobius translation(cx b);
    static Mobius invertAndReflect();
    static Mobius fromPoints(cx p, cx q, cx r);
    static Mobius fromPointsToPoints(cx p1, cx q1, cx r1, cx p2, cx q2, cx r2);
};

Mobius::Mobius() {
}

Mobius::Mobius(cx a_, cx b_, cx c_, cx d_) {
    cx sdet = sqrt(a_*d_ - b_*c_);
    a = a_/sdet;
    b = b_/sdet;
    c = c_/sdet;
    d = d_/sdet;
}

cx Mobius::apply(cx z) {
    return (a*z+b)/(c*z+d);
}

cx Mobius::applyInf() {
    return a/c;
}

void Mobius::print() {
    printf("(%lf, %lf) (%lf, %lf)\n(%lf, %lf) (%lf, %lf)\n\n",a.real(),a.imag(),b.real(),b.imag(),
            c.real(),c.imag(),d.real(),d.imag());
}

void printCx(cx z) {
    printf("(%lf, %lf)\n",z.real(),z.imag());
}

Mobius Mobius::scaling(cx a) {
    return Mobius(a, 0, 0, 1);
}

Mobius Mobius::translation(cx b) {
    return Mobius(1, b, 0, 1);
}

Mobius Mobius::invertAndReflect() {
    return Mobius(0, 1, 1, 0);
} 

// Takes point p -> 0, q -> 1, r -> infty
Mobius Mobius::fromPoints(cx p, cx q, cx r) {
    return Mobius(q-r,-p*(q-r),(q-p),-r*(q-p));
}

// Inverts the transformation
Mobius Mobius::inverse() {
    return Mobius(d,-b,-c,a);
}

// Composes the transformation with n,
// returning the transformation given by applying n
// before the transformation
Mobius Mobius::compose(Mobius n) {
    return Mobius(a*n.a + b*n.c, a*n.b + b*n.d, c*n.a + d*n.c, c*n.b + d*n.d); 
}

// Returns a transformation that takes points
// p1 -> p2, q1 -> q2, r1 -> r2
Mobius Mobius::fromPointsToPoints(cx p1, cx q1, cx r1, cx p2, cx q2, cx r2) {
    return fromPoints(p2, q2, r2).inverse().compose(fromPoints(p1, q1, r1));
}

Mobius Mobius::conjugate(Mobius s) {
    return s.inverse().compose(*this).compose(s);
}

Transform* Mobius::decompose() {
    if (fabs(c) > EPS) {
        Spherical* t = new Spherical();

        Mobius preInv = Mobius::translation(d/c);
        cx preOri = conj(preInv.apply(0));
        t->pre.o = Point(preOri);
        t->pre.x = Point(conj(preInv.apply(1))-preOri);
        t->pre.y = Point(conj(preInv.apply(1i))-preOri);

        Mobius postInv = Mobius::translation(a/c).compose(Mobius::scaling(-1.0/(c*c)));
        cx postOri = postInv.apply(0);
        t->post.o = Point(postOri);
        t->post.x = Point(postInv.apply(1)-postOri);
        t->post.y = Point(postInv.apply(1i)-postOri);

        return t;
    } else {
        Linear* t = new Linear();

        cx ori = apply(0);
        t->pre.o = Point(ori);
        t->pre.x = Point(apply(1)-ori);
        t->pre.y = Point(apply(1i)-ori);

        return t;
    }
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

Point Linear::apply(Point p) {
    return pre.apply(p);
}

struct Gasket {
    Mobius m[4];
    Gasket(cx p, cx q, cx r);
};

Gasket::Gasket(cx p, cx q, cx r) {
    Mobius s = Mobius::fromPointsToPoints(-1, 1i, 1, -1, 0, 1);
    cx pa = s.apply(-1+2*1i);
    cx pb = s.apply(1+2*1i);
    cx pc = s.applyInf();

    Mobius t = Mobius::fromPointsToPoints(p, q, r, -1, 0, 1);
    
    m[0] = Mobius::fromPointsToPoints(-1, 0, 1, -1, pa, pc).conjugate(t);
    m[1] = Mobius::fromPointsToPoints(-1, 0, 1, pa, 0, pb).conjugate(t);
    m[2] = Mobius::fromPointsToPoints(-1, 0, 1, pc, pb, 1).conjugate(t);
    m[3] = Mobius::scaling(-1).conjugate(t);
}

int main(int argc, char* argv[]) {
    if (argc < 7) {
        printf("Usage: %s [px] [py] [qx] [qy] [rx] [ry]\n", argv[0]);
        return 1;
    }

    cx p = cx(stod(argv[1]), stod(argv[2]));
    cx q = cx(stod(argv[3]), stod(argv[4]));
    cx r = cx(stod(argv[5]), stod(argv[6]));

    Gasket g(p, q, r);

    Transform* t[4];

    for (int i = 0; i < 4; i++) {
        t[i] = g.m[i].decompose();
        t[i]->print();
        delete t[i];
    }

    return 0;
}
