#ifndef SERVER_MYSERVER_H
#define SERVER_MYSERVER_H

#include "ipcsetup.h"
#include "MyConnectionServer.h"

class MyServer : public wxServer
{
    wxWindow *eventWindow;
public:
    explicit MyServer(wxWindow *eventWindow);
    virtual ~MyServer();

    void Disconnect();
    bool IsConnected() { return m_connection != NULL; }
    MyConnectionServer *GetConnection() { return m_connection; }

    void Advise();
    bool CanAdvise() { return m_connection && !m_connection->m_advise.empty(); }

    virtual wxConnectionBase *OnAcceptConnection(const wxString& topic) wxOVERRIDE;

protected:
    MyConnectionServer *m_connection;
};


#endif //SERVER_MYSERVER_H
