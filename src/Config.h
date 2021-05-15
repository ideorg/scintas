#ifndef SCINTAS_CONFIG_H
#define SCINTAS_CONFIG_H

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <vector>
#include "IniParser/IniParser.h"

class Config {
    std::string iniPath;
    IniParser *iniParser;
    const int mru_hard_limit = 50;
    int mru_limit = mru_hard_limit;
public:
    Config();
    ~Config();
    std::vector<wxString> mru;
    void UpdateMRUPageClose(wxString path);
    void UpdateMRUPageOpen(wxString path);
};

#endif //SCINTAS_CONFIG_H
