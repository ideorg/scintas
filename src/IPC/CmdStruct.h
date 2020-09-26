#ifndef SINGLE_CMDSTRUCT_H
#define SINGLE_CMDSTRUCT_H

#include <vector>
#include <wx/wx.h>
#include <wx/string.h>

struct CmdStruct {
    std::pair<char *, int> pack(std::vector<wxString> &argv);
    std::vector<wxString> unpack(char* p);
};


#endif //SINGLE_CMDSTRUCT_H
