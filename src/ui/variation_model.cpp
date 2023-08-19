#include "variation_model.hpp"

namespace ui {

VariationModel::VariationModel(wxDataViewListCtrl* variationCtrl): ViewModel(variationCtrl) { }

int VariationModel::getCount() const {
    return 2;
}

wxVariant VariationModel::getValue(int row, int col) const {
    std::string ans;
    int num = row*2+col;
    switch (num) {
        case 0: ans = "linear"; break;
        case 1: ans = "0.500000"; break;
        case 2: ans = "spherical"; break;
        case 3: ans = "0.500000"; break;
    }
    return ans;
}

}