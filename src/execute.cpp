#include "execute.h"
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>

using namespace std;

int execute(string executable, vector<string> params, bool wait) {
    int pid, status;
    if (pid = fork()) {
        if (wait)
            waitpid(pid, &status, 0); // wait for the child to exit
    } else {
        vector<char *> par;
        for (auto p: params) {
            char *p1  = new char[p.length()+1];
            strcpy(p1, p.c_str());
            par.push_back(p1);
        }
        execv(executable.c_str(), par.data());
        for (auto p1: par) delete p1;
    }
    return status; // this is the parent process again.
}
