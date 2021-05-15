#ifndef SCINTAS_CONFIG_H
#define SCINTAS_CONFIG_H

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <vector>

class Config {
    wxString iniPath;
public:
    std::vector<wxString> mru;
    Config();
};


#endif //SCINTAS_CONFIG_H
