#ifndef SCINTAS_MESSAGEBOX_H
#define SCINTAS_MESSAGEBOX_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/artprov.h>

#define wxYES_TO_ALL            0x00000080
#define wxNO_TO_ALL             0x00000100

class MessageBoxDialog: public wxDialog {
    void OnClick(wxCommandEvent &ev);
    unsigned buttonFlags;
    wxString imageId;
public:
    unsigned result;
    MessageBoxDialog(wxWindow *parent, wxWindowID id, const wxString &title, const wxString &message, unsigned buttonFlags, wxString imageId);
};

unsigned wxxMessageBox(const wxString &message, const wxString &title, unsigned buttonFlags, wxString imageId, wxWindow *parent=nullptr, wxWindowID id=wxID_ANY);

#endif //SCINTAS_MESSAGEBOX_H
