#ifndef CLIENT_MYCLIENT_H
#define CLIENT_MYCLIENT_H

#include "MyConnectionClient.h"
#include <vector>

class MyClient: public wxClient
{
public:
    MyClient();
    ~MyClient();
    bool Connect(const wxString& sHost, const wxString& sService, const wxString& sTopic);
    void Disconnect();
    wxConnectionBase *OnMakeConnection() wxOVERRIDE;
    bool IsConnected() { return m_connection != NULL; }
    MyConnectionClient *GetConnection() { return m_connection; }
    bool Connect();
    bool Poke(std::vector<wxString> &argv);
protected:
    MyConnectionClient     *m_connection;
};

#endif //CLIENT_MYCLIENT_H
