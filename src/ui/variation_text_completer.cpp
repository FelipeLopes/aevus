#include "variation_text_completer.hpp"
#include "../core/variation.hpp"
#include <vector>

using std::string;
using std::vector;

namespace ui {

VariationTextCompleter::VariationTextCompleter() {
    for (auto el: core::Variation::variationNames.right) {
        validStrings.insert(el.first);
    }
}

void VariationTextCompleter::GetCompletions(const wxString& prefix, wxArrayString& res) {
    vector<string> ans;
    string pref = prefix.ToStdString();
    if (pref.size() == 0) {
        return;
    }
    std::copy_if(validStrings.begin(), validStrings.end(), std::back_inserter(ans), [&](string s) {
        return s.starts_with(pref);
    });
    if (ans.size() == 1 && pref.compare(ans[0]) == 0) {
        ans.clear();
    }
    for (auto el: ans) {
        res.push_back(el);
    }
}

}