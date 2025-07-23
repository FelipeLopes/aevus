#pragma once

#include "view_model.hpp"
#include "content.hpp"
#include "../core/flame.hpp"
#include <boost/signals2.hpp>

namespace ui {

class FrameModel: public ViewModel {
public:
    FrameModel(wxDataViewListCtrl* frameListCtrl);
    void handleContent(FrameContent content);
    boost::signals2::signal<void (FrameContent)> frameContent;
private:
    FrameContent content;
    void getValues(std::vector<wxVector<wxVariant>>& data) const override;
    void setValue(const wxVariant& value, int row, int col) override;
};

}