#include "Config.h"
#include <unistd.h>

using namespace std;

Config::Config() {
#ifdef __unix__
    iniPath =  "/home/andrzej/.config/scintas/scintas.ini";
#else

#endif
    iniParser = new IniParser(iniPath, false);
    int count = iniParser->readInt32Def("MRUFiles","Count",0);
    mru.clear();
    for (int i=1; i<=count; i++)
        mru.push_back(iniParser->readStringDef("MRUFiles","File"+to_string(i),""));
}

Config::~Config() {
    delete iniParser;
}