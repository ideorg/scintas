#ifndef SCINTAS_MAIN_H
#define SCINTAS_MAIN_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/splitter.h>
#include "../3rdparty/auimod/auibook.h"
#include <wx/stc/stc.h>
#include <wx/statusbr.h>
#include <wx/snglinst.h>
#include <wx/timer.h>
#include <wx/fdrepdlg.h>
#include <wx/event.h>
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

enum wxxMenuID {
    wxxID_Editas  = wxID_HIGHEST + 1,
    wxxID_SaveAll,
    wxxID_SelectWord,
    wxxID_WordNext,
    wxxID_WordPrev,
    wxxInsertDate,
    wxxInsertTime,
    wxxInsertDateTime
};

class MyFrame : public wxFrame
{
    wxAuiManager manager;
    wxAuiNotebook* notebook;
    EditorFactory* editorFactory;
    wxTimer instanceTimer;
    Config *config;
    wxFindReplaceData findData;
    wxFindReplaceDialog* findDialog = nullptr;
    bool CloseAll();

    void OnExit(wxCommandEvent& event);
    void OnSelectWord(wxCommandEvent &event);
    void WordNextPrev(bool prev);
    void OnWordNext(wxCommandEvent &event);
    void OnWordPrev(wxCommandEvent &event);
    void OnEditas(wxCommandEvent &event);
    void OnStcMarginClick(wxStyledTextEvent& event);
    void OnStcModified(wxStyledTextEvent& event);
    void OnPageClose(wxAuiNotebookEvent &event);
    void SetEditorStyle(wxStyledTextCtrl* stc);
    void OnInstanceTimer(wxTimerEvent&);
    void CreateMenu();
    void OnCloseMain(wxCloseEvent& event);
DECLARE_EVENT_TABLE()
public:
    MyFrame(wxWindow *parent, wxWindowID id, const wxString &title,
            const wxPoint &pos, const wxSize &size, long style);
    ~MyFrame() { delete editorFactory;}
    void OnNewPage(wxCommandEvent& event);
    void OnOpenFile(wxCommandEvent& event);
    void OnSaveFile(wxCommandEvent &event);
    void OnSaveAs(wxCommandEvent &event);
    void OnSaveAll(wxCommandEvent &event);
    void OnClose(wxCommandEvent &event);
    void OnCloseAll(wxCommandEvent &event);
    void OnFind(wxCommandEvent& event);
    void OnDoFind(wxFindDialogEvent& event);
    void OnDoFindReplace(wxFindDialogEvent& event);
    void OnFindDialogClose(wxFindDialogEvent& WXUNUSED(event));
    int DoFind(wxStyledTextCtrl* stc, const wxString& str, int flags);
    void OnInsertDate(wxCommandEvent &event);
    void OnInsertTime(wxCommandEvent &event);
    void OnInsertDateTime(wxCommandEvent &event);
    void OpenInEditor(const wxString& file_path);
    void OpenOrActivate(const wxString& file_path);

    void CmdLineOpenFiles();
};

#endif //SCINTAS_MAIN_H
