#pragma once

#include <set>
#include <wx/textcompleter.h>

namespace ui {

class VariationTextCompleter: public wxTextCompleterSimple {
public:
    VariationTextCompleter();
    void GetCompletions(const wxString& prefix, wxArrayString& res) override;
private:
    std::set<std::string> validStrings;
};

}