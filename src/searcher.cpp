#include "searcher.hpp"
#include "key_gasket.hpp"
#include "sdf.hpp"

#include <boost/asio/post.hpp>
#include <gmpxx.h>
#include <memory>

using std::shared_ptr;
using std::array;
using std::vector;

template<typename T>
Searcher<T>::Searcher(shared_ptr<Scaler<T>> scaler_,
    Complex<T> pa_, Complex<T> pb_, Complex<T> pc_,
    Complex<T> center_,
    array<Mobius<T>, 3> transforms_,
    const vector<Mobius<T>>& input_,
    vector<KeyGasket>& output_,
    T ar_, int numThreads):
    pa(pa_), pb(pb_), pc(pc_), center(center_), transforms(transforms_), ar(ar_),
    scaler(scaler_), threadPool(numThreads), input(input_), output(output_) {

}

template <typename T>
int Searcher<T>::searchScale(Sdf<T> sdf) {
    int lb = 0;
    int ub = scaler->numSteps;
    while (ub - lb > 1) {
        int m = (lb + ub) / 2;
        T scale = scaler->lookupExp(m);
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
    Mobius<T> acc = input[i];
    auto qa = acc.apply(pa);
    auto qb = acc.apply(pb);
    auto qc = acc.apply(pc);
    Sdf<T> sdf = Sdf<T>::fromPoints(qa, qb, qc);
    int scaleVal = searchScale(sdf);
    KeyGasket g;
    T logscale = scaler->iniLogscale + scaleVal*scaler->step;
    g.logscale = toDouble(logscale);
    auto s = Mobius<T>::scaling(scaler->lookupExp(scaleVal))
        .compose(Mobius<T>::translation(-center))
        .compose(acc);

    for (int i=0; i<3; i++) {
        g.ifsTransforms.push_back(transforms[i].conjugate(s).toMobiusDouble());
    }

    lock.lock();
    if (scaleVal < scaler->numSteps) {
        if (output.size() < i+2) {
            output.resize(i+2);
        }
        output[i+1] = g;
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