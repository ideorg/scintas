#ifndef SERVER_MYCONNECTION_H
#define SERVER_MYCONNECTION_H

#include "MyConnectionBase.h"

class MyConnectionClient : public MyConnectionBase
{
public:
    virtual bool DoExecute(const void *data, size_t size, wxIPCFormat format) wxOVERRIDE;
    virtual const void *Request(const wxString& item, size_t *size = NULL, wxIPCFormat format = wxIPC_TEXT) wxOVERRIDE;
    virtual bool DoPoke(const wxString& item, const void* data, size_t size, wxIPCFormat format) wxOVERRIDE;
    virtual bool OnAdvise(const wxString& topic, const wxString& item, const void *data, size_t size, wxIPCFormat format) wxOVERRIDE;
};

#endif //SERVER_MYCONNECTION_H
