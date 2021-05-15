#include "Config.h"
#include <unistd.h>

using namespace std;

Config::Config() {
#ifdef __unix__
    iniPath =  "/home/andrzej/.config/scintas/scintas.ini";
#else

#endif
    iniParser = new IniParser(iniPath, false);
    mru_limit = min(iniParser->readInt32Def("MRUFiles","Max",mru_hard_limit), mru_hard_limit);
    int count = min(iniParser->readInt32Def("MRUFiles","Count",0), mru_limit);
    mru.clear();
    for (int i=1; i<=count; i++)
        mru.push_back(iniParser->readStringDef("MRUFiles","File"+to_string(i),""));
}

Config::~Config() {
    delete iniParser;
}

void Config::UpdateMRUPageClose(wxString path) {
    mru.insert(mru.begin(), path);
}

void Config::UpdateMRUPageOpen(wxString path) {

}
