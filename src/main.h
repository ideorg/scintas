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
#include <wx/snglinst.h>
#include <wx/timer.h>
#include "EditorFactory.h"
#include "Config.h"

enum {
    ID_NOTEBOOK
};

#define STC_LINE_NUM_MARGIN 0
#define STC_FOLD_MARGIN 1

class MyApp : public wxApp
{
    wxSingleInstanceChecker *m_checker;
public:
    virtual bool OnInit();
    int OnExit() override;
};

class MyFrame : public wxFrame
{
    wxAuiManager manager;
    wxAuiNotebook* notebook;
    EditorFactory* editorFactory;
    wxTimer instanceTimer;
    Config *config;

    void OnExit(wxCommandEvent& event);
    void OnStcMarginClick(wxStyledTextEvent& event);
    void SetEditorStyle(wxStyledTextCtrl* stc);
    void OnInstanceTimer(wxTimerEvent&);
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
