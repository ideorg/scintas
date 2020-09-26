#include "CmdStruct.h"
using namespace std;

pair<char *, int> CmdStruct::pack(vector<wxString> &argv) {
    int size = sizeof(int);
    for (wxString &arg: argv) {
        size += strlen(arg.c_str())+1;
    }
    char *p = new char[size];
    int n = argv.size();
    memcpy(p, &n, sizeof(n));
    int pos = sizeof(n);
    for (wxString &arg: argv) {
        int len = strlen(arg.c_str());
        memcpy(p+pos, arg.c_str(), len+1);
        pos += len+1;
    }
    return make_pair(p,size);
}

vector<wxString> CmdStruct::unpack(char *p) {
    vector<wxString> v;
    int n = *((int*)p);
    int pos = sizeof(int);
    for (int i=0; i<n; i++) {
        int len = strlen(p+pos);
        wxString arg = p+pos;
        v.push_back(arg);
        pos += len+1;
    }
    return v;
}
