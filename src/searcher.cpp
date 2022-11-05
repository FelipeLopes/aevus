#include "searcher.hpp"
#include "key_gasket.hpp"
#include "sdf.hpp"

#include <boost/asio/post.hpp>
#include <gmpxx.h>
#include <memory>

using std::array;
using std::vector;

template <typename T>
Searcher<T>::Searcher(const Shape<T>& shape_,
    const Scaler<T>& scaler_,
    Complex<T> center_,
    bool inverseDive_,
    const vector<Mobius<T>>& zoomTransforms_,
    vector<KeyGasket>& keyGaskets_,
    T ar_, int numThreads_):
    shape(shape_), center(center_), inverseDive(inverseDive_),
    ar(ar_), numThreads(numThreads_), scaler(scaler_), threadPool(numThreads_),
    zoomTransforms(zoomTransforms_), keyGaskets(keyGaskets_), lastPickedUp(numThreads_-1) {

    pts = shape.startingPoints(inverseDive);
    transforms = shape.diveArray(inverseDive);
}

template <typename T>
void Searcher<T>::start() {
    Sdf<T> sdf = Sdf<T>::fromPoints(pts[0], pts[1], pts[2]);
    T iniScale = scaler.lookupExp(0);
    T iniHeight = 2/iniScale;
    T iniWidth = iniHeight*ar;
    if (!sdf.rectInside(center, iniWidth, iniHeight)) {
        vector<Mobius<double>> gasketTransforms;
        auto transforms = shape.doubleSidedTransforms(scaler.lookupExp(0), center);
        gasketTransforms.insert(gasketTransforms.end(), transforms.begin(), transforms.end());
        KeyGasket g(gasketTransforms, 0);
        g.logscale = toDouble(scaler.iniLogscale);
        keyGaskets.push_back(g);
    }
    for (int i=0; i<numThreads; i++) {
        boost::asio::post(threadPool, [=] {
            task(i);
        });
    }
}

template <typename T>
void Searcher<T>::block() {
    threadPool.join();
}

template <typename T>
int Searcher<T>::searchScale(Sdf<T> sdf) {
    int lb = 0;
    int ub = scaler.numSteps;
    while (ub - lb > 1) {
        int m = (lb + ub) / 2;
        T scale = scaler.lookupExp(m);
        T height = 2/scale;
        T width = height*ar;
        if (sdf.rectInside(center, width, height)) {
            ub = m;
        } else {
            lb = m;
        }
    }
    return ub;
}

template<typename T>
void Searcher<T>::task(int i) {
    Mobius<T> acc = zoomTransforms[i];
    auto qa = acc.apply(pts[0]);
    auto qb = acc.apply(pts[1]);
    auto qc = acc.apply(pts[2]);
    Sdf<T> sdf = Sdf<T>::fromPoints(qa, qb, qc);
    int scaleVal = searchScale(sdf);

    T logscale = scaler.iniLogscale + scaleVal*scaler.step;
    auto s = Mobius<T>::scaling(scaler.lookupExp(scaleVal))
        .compose(Mobius<T>::translation(-center))
        .compose(acc);

    vector<Mobius<double>> gasketTransforms;
    for (int i=0; i<3; i++) {
        gasketTransforms.push_back(transforms[i].conjugate(s).toMobiusDouble());
    }
    KeyGasket g(gasketTransforms, 0);
    g.logscale = toDouble(logscale);

    lock.lock();
    if (scaleVal < scaler.numSteps) {
        if (keyGaskets.size() < i+2) {
            keyGaskets.resize(i+2);
        }
        keyGaskets[i+1] = g;
    } else {
        foundEnd = true;
    }
    if (!foundEnd) {
        lastPickedUp++;
        boost::asio::post(threadPool, [=] {
            task(lastPickedUp);
        });
    }
    lock.unlock();
}

template class Searcher<mpq_class>;