#include "MyClient.h"
#include "CmdStruct.h"

using namespace std;

MyClient::MyClient() : wxClient()
{
    m_connection = NULL;
}

bool MyClient::Connect(const wxString& sHost, const wxString& sService, const wxString& sTopic)
{
    wxLogNull nolog;

    m_connection = (MyConnectionClient *)MakeConnection(sHost, sService, sTopic);
    return m_connection    != NULL;
}

wxConnectionBase *MyClient::OnMakeConnection()
{
    return new MyConnectionClient;
}

void MyClient::Disconnect()
{
    if (m_connection)
    {
        m_connection->Disconnect();
        wxDELETE(m_connection);
    }
}

MyClient::~MyClient()
{
    Disconnect();
}

bool MyClient::Connect() {
    wxString servername = IPC_SERVICE;
    wxString hostname = IPC_HOST;
    wxString topic = IPC_TOPIC;
    bool retval = Connect(hostname, servername, topic);
    return retval;
}

bool MyClient::Poke(vector<wxString> &argv)
{
    if (IsConnected())
    {
        CmdStruct cmdStruct;
        auto pair = cmdStruct.pack(argv);
        GetConnection()->Poke("argv", pair.first, pair.second, wxIPC_TEXT);
        delete pair.first;
        return true;
    } else return false;
}

