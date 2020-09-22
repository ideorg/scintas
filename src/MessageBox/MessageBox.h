#ifndef SCINTAS_MESSAGEBOX_H
#define SCINTAS_MESSAGEBOX_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class MessageBox: public wxDialog {
    void OnClick(wxCommandEvent &ev);
public:
    MessageBox(wxWindow *parent, wxWindowID id, const wxString &title);
};


#endif //SCINTAS_MESSAGEBOX_H
