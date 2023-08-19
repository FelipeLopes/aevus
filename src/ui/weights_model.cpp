#include "weights_model.hpp"
#include "selection_view_model.hpp"
#include <string>

using std::to_string;

namespace ui {

WeightsModel::WeightsModel(wxDataViewListCtrl* weightsListCtrl):
    SelectionViewModel(weightsListCtrl) { }

int WeightsModel::getCount() const {
    return 3;
}

wxVariant WeightsModel::getValue(int row, int col) const {
    if (col == 0) {
        return to_string(row + 1);
    }
    return to_string((row + 1)*0.3);
}

void WeightsModel::setValue(const wxVariant& val, int row, int col) {
    update();
}

}