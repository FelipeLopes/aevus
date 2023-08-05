#include "serializable.hpp"
#include <iomanip>
#include <sstream>

using std::setprecision;
using std::string;
using std::stringstream;

namespace serial {

string formattedDouble(double x) {
    stringstream buffer;
    buffer<<setprecision(15)<<x;
    auto ans = buffer.str();
    if (ans == "-0") {
        ans = "0";
    }
    return ans;
}

}