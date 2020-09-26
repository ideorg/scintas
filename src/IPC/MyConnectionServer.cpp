#include "MyConnectionServer.h"
#include "CmdStruct.h"

bool MyConnectionServer::OnPoke(const wxString& topic,
                                const wxString& item,
                                const void *data,
                                size_t size,
                                wxIPCFormat format)
{
    Log("[server] OnPoke", topic, item, data, size, format);
    CmdStruct cmdStruct;
    auto argv = cmdStruct.unpack((char*)data);
    for (auto &arg: argv)
        wxLogMessage("[server] argv \"%s\"", arg);
    wxCommandEvent cmdevent(wxEVT_BUTTON, ID_SERVER_ONPOKE);
    cmdevent.SetEventObject(eventWindow);
    eventWindow->HandleWindowEvent(cmdevent);
    return true;
}

const void * MyConnectionServer::OnRequest(const wxString& topic,
                                           const wxString& item,
                                           size_t *size,
                                           wxIPCFormat format)
{
    *size = 0;

    wxString s,
            afterDate;
    if ( item.StartsWith("Date", &afterDate) )
    {
        const wxDateTime now = wxDateTime::Now();

        if ( afterDate.empty() )
        {
            s = now.Format();
            *size = wxNO_LEN;
        }
        else if ( afterDate == "+len" )
        {
            s = now.FormatTime() + " " + now.FormatDate();
            *size = strlen(s.mb_str()) + 1;
        }
    }
    else if ( item == "bytes[3]" )
    {
        s = "123";
        *size = 3;
    }

    if ( !*size )
    {
        wxLogMessage("[server] Unknown request for \"%s\"", item);
        return NULL;
    }

    // store the data pointer to which we return in a member variable to ensure
    // that the pointer remains valid even after we return
    m_requestData = s.mb_str();
    const void * const data = m_requestData;
    Log("[server] OnRequest", topic, item, data, *size, format);
    return data;
}

bool MyConnectionServer::DoAdvise(const wxString& item,
                                  const void *data,
                                  size_t size,
                                  wxIPCFormat format)
{
    Log("[server] Advise", "", item, data, size, format);
    return wxConnection::DoAdvise(item, data, size, format);
}
