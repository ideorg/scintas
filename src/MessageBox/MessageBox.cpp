#include "MessageBox.h"
#include <wx/imaglist.h>
#include <wx/artprov.h>

MessageBox::MessageBox(wxWindow *parent, wxWindowID id, const wxString &title)
: wxDialog(parent, id, title, wxDefaultPosition, wxSize(300,100), wxDEFAULT_FRAME_STYLE) {
    wxPanel* p = new wxPanel(this, wxID_ANY);

    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL );
    wxBoxSizer *top_box = new wxBoxSizer( wxHORIZONTAL );
    const wxSize imageSize(32, 32);
    top_box->Add(
            new wxStaticBitmap(p, wxID_ANY,
                               wxArtProvider::GetIcon(wxART_QUESTION, wxART_OTHER, imageSize)),
            wxSizerFlags().Border(wxALL, 7));
    top_box->Add(
            new wxStaticText(p, wxID_ANY, "1234567890"),
            wxSizerFlags().Border(wxALL, 7));

    wxBoxSizer *button_box = new wxBoxSizer( wxHORIZONTAL );
    button_box->Add(
            new wxButton( p, wxID_ANY, "Two buttons in a box" ),
            wxSizerFlags().Border(wxALL, 7));
    button_box->Add(
            new wxButton( p, wxID_ANY, "(wxCENTER)" ),
            wxSizerFlags().Border(wxALL, 7));

    sizer->Add(top_box, wxSizerFlags().Center());
    sizer->Add(button_box, wxSizerFlags().Center());

    p->SetSizer(sizer );

    // don't allow frame to get smaller than what the sizers tell it and also set
    // the initial size as calculated by the sizers
    sizer->SetSizeHints(this );
}
