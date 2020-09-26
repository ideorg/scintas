#ifndef SERVER_MYCONNECTIONSERVER_H
#define SERVER_MYCONNECTIONSERVER_H

#include "MyConnectionBase.h"

#define ID_SERVER_ONPOKE 10000

class MyConnectionServer : public MyConnectionBase
{
    wxWindow *eventWindow;
public:
    explicit MyConnectionServer(wxWindow *eventWindow):eventWindow(eventWindow),MyConnectionBase(){}
    virtual const void *OnRequest(const wxString& topic, const wxString& item, size_t *size, wxIPCFormat format) wxOVERRIDE;
    virtual bool OnPoke(const wxString& topic, const wxString& item, const void *data, size_t size, wxIPCFormat format) wxOVERRIDE;
    virtual bool DoAdvise(const wxString& item, const void *data, size_t size, wxIPCFormat format) wxOVERRIDE;

    // topic for which we advise the client or empty if none
    wxString m_advise;

protected:
    // the data returned by last OnRequest(): we keep it in this buffer to
    // ensure that the pointer we return from OnRequest() stays valid
    wxCharBuffer m_requestData;
};

#endif //SERVER_MYCONNECTIONSERVER_H
