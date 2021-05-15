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
#include <wx/timer.h>
#include <wx/fdrepdlg.h>
#include <wx/event.h>
#include "EditorFactory.h"
#include "Config.h"
#include "IPC/MyServer.h"

enum {
    ID_NOTEBOOK
};

#define STC_LINE_NUM_MARGIN 0
#define STC_FOLD_MARGIN 1

class MyApp : public wxApp
{
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
    wxxID_Goto,
    wxxID_Reopen,
    wxxInsertDate,
    wxxInsertTime,
    wxxInsertDateTime,
    wxxID_ChangeCaseUpper,
    wxxID_ChangeCaseLower,
    wxxID_ChangeCaseInvert,
    wxxID_ChangeCaseUpperFirst,
    wxxID_Window,
    wxxID_MRU=wxxID_Window+10+26
};

class MyFrame : public wxFrame
{
    MyServer *m_server;
    bool StartServer();
    void OnPoke(wxCommandEvent &event);
    enum ChangeCaseEnum {ccUpper, ccLower};

    wxMenu *recent_file;
    wxMenu *menuWindow;
    wxAuiManager manager;
    wxAuiNotebook* notebook;
    EditorFactory* editorFactory;
    Config *config;
    wxFindReplaceData findData;
    wxFindReplaceDialog* findDialog = nullptr;
    bool CloseAll();

    void OnExit(wxCommandEvent& event);
    wxStyledTextCtrl *SelectWord();
    void OnSelectWord(wxCommandEvent &event);
    void ChangeCase(ChangeCaseEnum cc);
    void OnChangeCaseUpper(wxCommandEvent &event);
    void OnChangeCaseLower(wxCommandEvent &event);
    void OnCut(wxCommandEvent &event);
    void OnCopy(wxCommandEvent &event);
    void OnPaste(wxCommandEvent &event);
    void OnDelete(wxCommandEvent &event);
    void OnSelectAll(wxCommandEvent &event);
    void WordNextPrev(bool prev);
    void OnWordNext(wxCommandEvent &event);
    void OnWordPrev(wxCommandEvent &event);
    void OnEditas(wxCommandEvent &event);
    void OnStcMarginClick(wxStyledTextEvent& event);
    void OnStcModified(wxStyledTextEvent& event);
    void OnPageClose(wxAuiNotebookEvent &event);
    //void SetEditorStyle(wxStyledTextCtrl* stc);
    void CreateMenu();
    void UpdateMenuWindow();
    void UpdateMenuMRU();
    void UpdateMRUPageClose(wxString path);
    void UpdateMRUPageOpen(wxString path);
    void OnCloseMain(wxCloseEvent& event);
    void OnWindow(wxCommandEvent& event);
    void OnMRU(wxCommandEvent& event);

DECLARE_EVENT_TABLE()
public:
    MyFrame(wxWindow *parent, wxWindowID id, const wxString &title,
            const wxPoint &pos, const wxSize &size, long style);
    ~MyFrame() { delete editorFactory;}
    void OnNewPage(wxCommandEvent& event);
    void OnOpenFile(wxCommandEvent& event);
    void OnReopenFile(wxCommandEvent &event);
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
    void OnGoto(wxCommandEvent& event);
    void OnInsertDate(wxCommandEvent &event);
    void OnInsertTime(wxCommandEvent &event);
    void OnInsertDateTime(wxCommandEvent &event);
    void OpenInEditor(const wxString& file_path);
    void OpenOrActivate(const wxString& file_path);
    void OnKeyDown(wxKeyEvent& event);

    void CmdLineOpenFiles();
};

#endif //SCINTAS_MAIN_H
