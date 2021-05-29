#include "execute.h"
#ifdef __unix__
#include <unistd.h>
#include <sys/wait.h>
#endif
#include <cstring>

using namespace std;

int execute(string executable, vector<string> params, bool wait) {
    string cmdline = "";
    for (auto p: params)
        cmdline+=" "+string(p.c_str());
    system((executable+cmdline).c_str());
    return 0;
}
