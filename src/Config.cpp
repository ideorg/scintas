#include "Config.h"
#include <unistd.h>

using namespace std;

Config::Config() {
    string appPath;
#ifdef __unix__
    char buf[4096];
    ssize_t len = readlink("/proc/self/exe", buf, 4095);
    buf[len] = 0;
    appPath = buf;
#else
    Windows: GetModuleFileName
    error
    int n = appPath.find_last_of('.');
    if (n!=string::npos)
        appPath = appPath.substr(0,n);
#endif
    iniPath =  appPath + ".ini";
}