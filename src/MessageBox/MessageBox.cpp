#include "MessageBox.h"
#include <wx/imaglist.h>

void MessageBox::OnClick(wxCommandEvent &ev) {
    result = ev.GetId();
    this->Close();
}

const wxString captions[] {
    "","Yes","OK","No","Cancel","Apply", "Close",
    "Yes to all","No to all"
};

MessageBox::MessageBox(wxWindow *parent, wxWindowID id, const wxString &title, const wxString &message, unsigned buttonFlags, wxString imageId)
: wxDialog(parent, id, title, wxDefaultPosition, wxSize(300,100), wxDEFAULT_FRAME_STYLE),
buttonFlags(buttonFlags),imageId(imageId) {
    if (!buttonFlags) buttonFlags = 1;
    wxPanel* p = new wxPanel(this, wxID_ANY);

    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL );
    wxBoxSizer *top_box = new wxBoxSizer( wxHORIZONTAL );
    const wxSize imageSize(32, 32);
    top_box->Add(
            new wxStaticBitmap(p, wxID_ANY,
                               wxArtProvider::GetIcon(imageId, wxART_OTHER, imageSize)),
            wxSizerFlags().Border(wxALL, 7));
    top_box->Add(
            new wxStaticText(p, wxID_ANY, message),
            wxSizerFlags().Border(wxALL, 7));

    wxBoxSizer *button_box = new wxBoxSizer( wxHORIZONTAL );
    for (int idx=0; idx<=8; idx++) {
        unsigned idb = 1<<idx;
        if ((buttonFlags & idb) == 0 )continue;
        button_box->Add(
                new wxButton( p, idb, captions[idx]),
                wxSizerFlags().Border(wxALL, 7));
        Bind(wxEVT_BUTTON,&MessageBox::OnClick, this, idb);
    }
    sizer->Add(top_box, wxSizerFlags().Center());
    sizer->Add(button_box, wxSizerFlags().Center());
    p->SetSizer(sizer );
    // don't allow frame to get smaller than what the sizers tell it and also set
    // the initial size as calculated by the sizers
    sizer->SetSizeHints(this );
    result = wxCANCEL;
}

unsigned wxxMessageBox(const wxString &title, const wxString &message, unsigned buttonFlags, wxString imageId, wxWindow *parent, wxWindowID id) {
    MessageBox *mesageBox = new MessageBox(parent, id, title,message,buttonFlags, wxART_WARNING);
    mesageBox->ShowModal();
    unsigned result = mesageBox->result;
    delete mesageBox;
    return result;
}