#include "MyServer.h"

MyServer::MyServer(wxWindow *eventWindow):eventWindow(eventWindow),wxServer(){
    m_connection = NULL;
}

MyServer::~MyServer()
{
    Disconnect();
}

wxConnectionBase *MyServer::OnAcceptConnection(const wxString& topic)
{
    if ( topic == IPC_TOPIC )
    {
        m_connection = new MyConnectionServer(eventWindow);
        return m_connection;
    }
    //else: unknown topic

    wxLogMessage("[server] Unknown topic, connection refused");
    return NULL;
}

void MyServer::Disconnect()
{
    if ( m_connection )
    {
        //wxDELETE(m_connection);
        //wxLogMessage("[server] Disconnected client");
        //commented because is already disconnected
    }
}

void MyServer::Advise()
{
    if ( CanAdvise() )
    {
        const wxDateTime now = wxDateTime::Now();

        wxString str = wxString::FromUTF8("\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82");
        m_connection->Advise(m_connection->m_advise, str + " (using UTF-8)");

        str += " (using wchar_t)";
        m_connection->Advise(m_connection->m_advise,
                             str.wc_str(), (str.length() + 1)*sizeof(wchar_t),
                             wxIPC_UNICODETEXT);

        // This one uses wxIPC_TEXT by default.
        const wxString s = now.FormatTime() + " " + now.FormatDate();
        m_connection->Advise(m_connection->m_advise, s.mb_str(), wxNO_LEN);

        char bytes[3] = { '1', '2', '3' };
        m_connection->Advise(m_connection->m_advise, bytes, 3, wxIPC_PRIVATE);
    }
}
