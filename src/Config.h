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
public:
    std::vector<wxString> mru;
    Config();
    ~Config();
};


#endif //SCINTAS_CONFIG_H
