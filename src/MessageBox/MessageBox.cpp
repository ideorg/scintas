#include "MessageBox.h"
#include <wx/imaglist.h>
#include <wx/artprov.h>

MessageBox::MessageBox(wxWindow *parent, wxWindowID id, const wxString &title)
: wxDialog(parent, id, title, wxDefaultPosition, wxSize(300,100), wxDEFAULT_FRAME_STYLE) {
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *sizerUp = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *label = new wxStaticText(this, wxID_ANY, "1234567890");

    const wxSize imageSize(32, 32);
    wxStaticBitmap *sbmp = new wxStaticBitmap(this, wxID_ANY,
                                              wxArtProvider::GetIcon(wxART_QUESTION, wxART_OTHER, imageSize),
                                              wxPoint(10,10), wxSize(32,32));

    sizerUp->Add(sbmp, 1,wxCENTER);
    sizerUp->Add(label, 1, wxRIGHT);

    wxBoxSizer *sizerButtons = new wxBoxSizer(wxHORIZONTAL);
    wxButton *button = new wxButton(this, 1, "OK");
    sizerButtons->Add(button, 1,wxCENTER);
    wxButton *button1 = new wxButton(this, 1, "1");
    sizerButtons->Add(button1, 1,wxCENTER);
    sizer->Add(sizerUp);
    sizer->Add(sizerButtons);
    SetSizer(sizer);
}
