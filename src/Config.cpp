#include "Config.h"
#ifdef __unix__
#include <unistd.h>
#endif
#include <algorithm>

Config::Config() {
#ifdef __unix__
    iniPath =  "/home/andrzej/.config/scintas/scintas.ini";
#else
    iniPath = "c:/Users/boruc/.config/scintas/scintas.ini";

#endif
    iniParser = new IniParser(iniPath, false);
    mru_limit = std::min(iniParser->readInt32Def("MRUFiles","Max",mru_def_limit), mru_hard_limit);
    int count = std::min(iniParser->readInt32Def("MRUFiles","Count",0), mru_limit);
    mru.clear();
    for (int i=1; i<=count; i++)
        mru.push_back(iniParser->readStringDef("MRUFiles","File"+std::to_string(i),""));
}

void Config::Flush() {
    iniParser->beginUpdate();
    iniParser->deleteSection("MRUFiles");
    iniParser->writeLong("MRUFiles", "Max", mru_limit);
    iniParser->writeLong("MRUFiles", "Count", mru.size());
    for (int i=0; i<mru.size(); i++)
        iniParser->writeString("MRUFiles","File"+std::to_string(i+1),mru[i].ToStdString());
    iniParser->endUpdate();
}

Config::~Config() {
    Flush();
    delete iniParser;
}

void Config::UpdateMRUPageClose(wxString path) {
    mru.erase(std::remove(mru.begin(), mru.end(), path), mru.end());
    if (mru.size()>=mru_limit) mru.resize(mru_limit-1);
    mru.insert(mru.begin(), path);
}

void Config::UpdateMRUPageOpen(wxString path) {
    mru.erase(std::remove(mru.begin(), mru.end(), path), mru.end());
}
