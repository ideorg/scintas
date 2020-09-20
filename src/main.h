#ifndef SCINTAS_MAIN_H
#define SCINTAS_MAIN_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/splitter.h>
#include <wx/aui/auibook.h>
#include <wx/stc/stc.h>
#include <wx/statusbr.h>
#include "EditorFactory.h"

enum {
    ID_NOTEBOOK
};

#define STC_LINE_NUM_MARGIN 0
#define STC_FOLD_MARGIN 1

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

class MyFrame : public wxFrame
{
    wxAuiManager manager;
    wxAuiNotebook* notebook;
    EditorFactory* editorFactory;

    void OnExit(wxCommandEvent& event);
    void OnStcMarginClick(wxStyledTextEvent& event);
    void SetEditorStyle(wxStyledTextCtrl* stc);
DECLARE_EVENT_TABLE()
public:
    MyFrame(wxWindow *parent, wxWindowID id, const wxString &title,
            const wxPoint &pos, const wxSize &size, long style);
    ~MyFrame() { delete editorFactory;}
    void OnOpenFile(wxCommandEvent& event);
    void OpenInEditor(const wxString& file_path);
    void OpenOrActivate(const wxString& file_path);

    void CmdLineOpenFiles();
};

#endif //SCINTAS_MAIN_H
