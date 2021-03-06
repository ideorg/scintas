#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "MessageBox.h"

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

class MyFrame : public wxFrame
{
public:
    MyFrame();
private:
    wxCheckBox *cbYES;
    wxCheckBox *cbOK;
    wxCheckBox *cbNO;
    wxCheckBox *cbCANCEL;
    wxCheckBox *cbAPPLY;
    wxCheckBox *cbCLOSE;
    wxCheckBox *cbYES_TO_ALL;
    wxCheckBox *cbNO_TO_ALL;
    int getFlags();
    void OnWx(wxCommandEvent& event);
    void OnNew(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
};
enum
{
    ID_Wx = 1,
    ID_New = 2
};
wxIMPLEMENT_APP(MyApp);
bool MyApp::OnInit()
{
    wxInitAllImageHandlers();
    MyFrame *frame = new MyFrame();
    frame->Show(true);
    return true;
}
MyFrame::MyFrame()
        : wxFrame(NULL, wxID_ANY, "Hello World")
{
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_Wx, "&Wx standard dialog", "");
    menuFile->Append(ID_New, "&New dialog", "");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    SetMenuBar( menuBar );
    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");
    Bind(wxEVT_MENU, &MyFrame::OnWx, this, ID_Wx);
    Bind(wxEVT_MENU, &MyFrame::OnNew, this, ID_New);
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    cbYES = new wxCheckBox(this, 1, "wxYES");
    sizer->Add(cbYES);
    cbOK = new wxCheckBox(this, 1, "wxOK");
    sizer->Add(cbOK);
    cbNO = new wxCheckBox(this, 1, "wxNO");
    sizer->Add(cbNO);
    cbCANCEL = new wxCheckBox(this, 1, "wxCANCEL");
    sizer->Add(cbCANCEL);
    cbAPPLY = new wxCheckBox(this, 1, "wxAPPLY");
    sizer->Add(cbAPPLY);
    cbCLOSE = new wxCheckBox(this, 1, "wxCLOSE");
    sizer->Add(cbCLOSE);
    cbYES_TO_ALL = new wxCheckBox(this, 1, "wxYES_TO_ALL");
    sizer->Add(cbYES_TO_ALL);
    cbNO_TO_ALL = new wxCheckBox(this, 1, "wxNO_TO_ALL");
    sizer->Add(cbNO_TO_ALL);
    SetSizer(sizer);
}

int MyFrame::getFlags() {
    int flags = 0;
    if (cbYES->IsChecked()) flags |= wxYES;
    if (cbOK->IsChecked()) flags |= wxOK;
    if (cbNO->IsChecked()) flags |= wxNO;
    if (cbCANCEL->IsChecked()) flags |= wxCANCEL;
    if (cbAPPLY->IsChecked()) flags |= wxAPPLY;
    if (cbCLOSE->IsChecked()) flags |= wxCLOSE;
    if (cbYES_TO_ALL->IsChecked()) flags |= wxYES_TO_ALL;
    if (cbNO_TO_ALL->IsChecked()) flags |= wxNO_TO_ALL;
    return flags;
}

void MyFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void MyFrame::OnWx(wxCommandEvent& event)
{
    wxMessageBox("message", "caption",getFlags(), this);
}

void MyFrame::OnNew(wxCommandEvent& event)
{
    wxString message = "The text in the \"Untitled1\" file has been changed.\n\n";
    message += "Do you want to save the modifications? (No = close and discard changes)";
    unsigned result = wxxMessageBox("warning", message, getFlags(), wxART_WARNING);
    wxMessageBox(wxString::Format(wxT("%i"),result));
}
