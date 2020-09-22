#include "MessageBox.h"
#include <wx/imaglist.h>
#include <wx/artprov.h>

MessageBox::MessageBox(wxWindow *parent, wxWindowID id, const wxString &title)
: wxDialog(parent, id, title, wxDefaultPosition, wxSize(300,100), wxDEFAULT_FRAME_STYLE) {
    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton *button = new wxButton(this, 1, "OK");
    sizer->Add(button);
    const wxSize imageSize(32, 32);
    wxStaticBitmap *sbmp = new wxStaticBitmap(this, wxID_ANY,
                                              wxArtProvider::GetIcon(wxART_QUESTION, wxART_OTHER, imageSize),
    wxPoint(10,10), wxSize(32,32));
    sizer->Add(sbmp);
    SetSizer(sizer);
}
