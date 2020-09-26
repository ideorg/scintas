#include "MyConnectionClient.h"

bool MyConnectionClient::OnAdvise(const wxString& topic, const wxString& item, const void *data,
                            size_t size, wxIPCFormat format)
{
    return true;
}

bool MyConnectionClient::DoExecute(const void *data, size_t size, wxIPCFormat format)
{
    bool retval = wxConnection::DoExecute(data, size, format);
    return retval;
}

const void *MyConnectionClient::Request(const wxString& item, size_t *size, wxIPCFormat format)
{
    const void *data =  wxConnection::Request(item, size, format);
    return data;
}

bool MyConnectionClient::DoPoke(const wxString& item, const void *data, size_t size, wxIPCFormat format)
{
    return wxConnection::DoPoke(item, data, size, format);
}
