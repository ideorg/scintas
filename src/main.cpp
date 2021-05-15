#include "main.h"
#include <wx/filename.h>
#include <string>
#include <vector>
#include "execute.h"
#include "MessageBox/MessageBox.h"
#include "MyTabArt.h"
#include <wx/clipbrd.h>
#include <wx/textdlg.h>
#include <wx/msgdlg.h>
#include "IPC/MyClient.h"
#include "IPC/CmdStruct.h"

using namespace std;

wxIMPLEMENT_APP(MyApp);
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
                EVT_MENU(wxID_EXIT, MyFrame::OnExit)
                EVT_MENU(wxxID_Editas, MyFrame::OnEditas)
                EVT_MENU(wxxID_SelectWord, MyFrame::OnSelectWord)
                EVT_MENU(wxxID_WordNext, MyFrame::OnWordNext)
                EVT_MENU(wxxID_WordPrev, MyFrame::OnWordPrev)
                EVT_MENU(wxID_OPEN, MyFrame::OnOpenFile)
                EVT_MENU(wxxID_Reopen, MyFrame::OnReopenFile)
                EVT_MENU(wxID_NEW, MyFrame::OnNewPage)
                EVT_MENU(wxID_SAVE, MyFrame::OnSaveFile)
                EVT_MENU(wxID_SAVEAS, MyFrame::OnSaveAs)
                EVT_MENU(wxxID_SaveAll, MyFrame::OnSaveAll)
                EVT_MENU(wxID_CLOSE, MyFrame::OnClose)
                EVT_MENU(wxID_CLOSE_ALL, MyFrame::OnCloseAll)
                EVT_MENU(wxID_CUT, MyFrame::OnCut)
                EVT_MENU(wxID_COPY, MyFrame::OnCopy)
                EVT_MENU(wxID_PASTE, MyFrame::OnPaste)
                EVT_MENU(wxID_DELETE, MyFrame::OnDelete)
                EVT_MENU(wxID_SELECTALL, MyFrame::OnSelectAll)
                EVT_MENU(wxID_FIND, MyFrame::OnFind)
                EVT_MENU(wxID_REPLACE, MyFrame::OnFind)
                EVT_MENU(wxxID_Goto, MyFrame::OnGoto)
                EVT_MENU(wxxInsertDate, MyFrame::OnInsertDate)
                EVT_MENU(wxxInsertTime, MyFrame::OnInsertTime)
                EVT_MENU(wxxInsertDateTime, MyFrame::OnInsertDateTime)
                EVT_MENU(wxxID_ChangeCaseUpper, MyFrame::OnChangeCaseUpper)
                EVT_MENU(wxxID_ChangeCaseLower, MyFrame::OnChangeCaseLower)
                EVT_FIND(wxID_ANY, MyFrame::OnDoFind)
                EVT_FIND_NEXT(wxID_ANY, MyFrame::OnDoFind)
                EVT_FIND_REPLACE(wxID_ANY, MyFrame::OnDoFindReplace)
                EVT_FIND_REPLACE_ALL(wxID_ANY, MyFrame::OnDoFindReplace)
                EVT_FIND_CLOSE(wxID_ANY, MyFrame::OnFindDialogClose)
                EVT_BUTTON(ID_SERVER_ONPOKE,MyFrame::OnPoke)
END_EVENT_TABLE()

bool MyApp::OnInit() {
    auto client = new MyClient;
    if (client->Connect()) {
        wxApp &app = wxGetApp();
        vector<wxString> argv;
        for (int i=0; i<app.argc; i++)
            argv.push_back(app.argv[i]);
        client->Poke(argv);
        wxDELETE(client);
        return false;
    }
    wxDELETE(client);
    MyFrame *frame = new MyFrame(NULL, wxID_ANY, "Scintas", wxDefaultPosition, wxSize(800,600), wxDEFAULT_FRAME_STYLE);;
    frame->Show(true);
    return true;
}

int MyApp::OnExit()
{
    return 0;
}

void MyFrame::OnPoke(wxCommandEvent &event) {
    SetFocus();
    CmdStruct cmdStruct;
    char *data = (char*)event.GetClientData();
    auto argv = cmdStruct.unpack(data);
    for (int i=1; i<argv.size(); i++)
        OpenOrActivate(argv[i]);
}

bool MyFrame::StartServer()
{
    // Create a new server
    m_server = new MyServer(this);
    wxString servername = IPC_SERVICE;
    if (m_server->Create(servername))
    {
        return true;
    }
    else
    {
        wxLogMessage("[server] Server %s failed to start", servername);
        wxDELETE(m_server);
        return false;
    }
    //UpdateUI();
}

void MyFrame::CreateMenu() {
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(wxID_NEW);
    wxMenuItem *open_file = new wxMenuItem(menuFile, wxID_OPEN, "Open file\tCtrl-O", "");
    menuFile->Append(open_file);
    wxMenuItem *recent_file = new wxMenuItem(menuFile, wxID_OPEN, "Recent files", "");
    menuFile->Append(recent_file);
    wxMenuItem *file_according_name = new wxMenuItem(menuFile, wxID_OPEN, "According to name", "");
    menuFile->Append(file_according_name);
    wxMenuItem *file_according_path = new wxMenuItem(menuFile, wxID_OPEN, "According to path", "");
    menuFile->Append(file_according_path);
    wxMenuItem *file_reopen = new wxMenuItem(menuFile, wxxID_Reopen, "&Reopen file", "");
    menuFile->Append(file_reopen);
    menuFile->Append(wxID_SEPARATOR);
    menuFile->Append(wxID_SAVE);
    menuFile->Append(wxID_SAVEAS);
    wxMenuItem *file_saveall = new wxMenuItem(menuFile, wxxID_SaveAll, "Save all\tCtrl-Shift-S", "");
    menuFile->Append(file_saveall);
    wxMenuItem *file_close = new wxMenuItem(menuFile, wxID_CLOSE, "Close file\tCtrl-F4", "");
    menuFile->Append(file_close);
    wxMenuItem *file_closeall = new wxMenuItem(menuFile, wxID_CLOSE_ALL, "Close all\tCtrl-Shift-F4", "");
    menuFile->Append(file_closeall);
    wxMenuItem *exit_file = new wxMenuItem(menuFile, wxID_EXIT, "E&xit\tAlt-F4", "");
    menuFile->Append(exit_file);

    wxMenu *menuEdit = new wxMenu;
    menuEdit->Append(wxID_UNDO);
    menuEdit->Append(wxID_REDO);
    menuEdit->Append(wxID_SEPARATOR);
    wxMenuItem *edit_cut = new wxMenuItem(menuFile, wxID_CUT, "Cut\tCtrl-X", "");
    menuEdit->Append(edit_cut);
    wxMenuItem *edit_copy = new wxMenuItem(menuFile, wxID_COPY, "Copy\tCtrl-C", "");
    menuEdit->Append(edit_copy);
    wxMenuItem *edit_paste = new wxMenuItem(menuFile, wxID_PASTE, "Paste\tCtrl-V", "");
    menuEdit->Append(edit_paste);
    wxMenuItem *edit_delete = new wxMenuItem(menuFile, wxID_DELETE, "Delete", "");
    menuEdit->Append(edit_delete);
    wxMenuItem *edit_selectAll = new wxMenuItem(menuFile, wxID_SELECTALL, "Select all\tCtrl+A", "");
    menuEdit->Append(edit_selectAll);
    wxMenuItem *edit_selectWord = new wxMenuItem(menuFile, wxxID_SelectWord, "Select word\tCtrl+W", "");
    menuEdit->Append(edit_selectWord);
    wxMenuItem *edit_selectMode = new wxMenuItem(menuFile, wxID_ANY, "Select mode", "");
    menuEdit->Append(edit_selectMode);
    wxMenuItem *edit_changeCase = new wxMenuItem(menuFile, wxID_ANY, "Change case", "");
    wxMenu *menuChangeCase = new wxMenu;
    edit_changeCase->SetSubMenu(menuChangeCase);
    wxMenuItem *edit_changeCaseUpper = new wxMenuItem(menuFile, wxxID_ChangeCaseUpper, "Upper\tCtrl-U", "");
    menuChangeCase->Append(edit_changeCaseUpper);
    wxMenuItem *edit_changeCaseLower = new wxMenuItem(menuFile, wxxID_ChangeCaseLower, "Lower\tCtrl-L", "");
    menuChangeCase->Append(edit_changeCaseLower);
    menuEdit->Append(edit_changeCase);

    wxMenu *menuSearch = new wxMenu;
    menuSearch->Append(wxID_FIND);
    wxMenuItem *search_findNext = new wxMenuItem(menuSearch, wxID_OPEN, "Find next", "");
    menuSearch->Append(search_findNext);
    wxMenuItem *search_wordPrev = new wxMenuItem(menuSearch, wxxID_WordPrev, "Word next\tCtrl-Alt-Up", "");
    wxAcceleratorEntry accelUp;
    accelUp.Set(wxACCEL_ALT|wxACCEL_CTRL, WXK_UP,wxxID_WordNext);
    search_wordPrev->SetAccel(&accelUp);
    menuSearch->Append(search_wordPrev);
    wxMenuItem *search_wordNext = new wxMenuItem(menuSearch, wxxID_WordNext, "Word next\tCtrl-Alt-Down", "");
    wxAcceleratorEntry accelDown;
    accelDown.Set(wxACCEL_ALT | wxACCEL_CTRL, WXK_DOWN, wxxID_WordNext);
    search_wordNext->SetAccel(&accelDown);
    menuSearch->Append(search_wordNext);
    menuSearch->Append(wxID_SEPARATOR);
    wxMenuItem *search_goto = new wxMenuItem(menuSearch, wxxID_Goto, "Goto\tCtrl-G", "");
    menuSearch->Append(search_goto);

    wxMenu *menuTools = new wxMenu;
    wxMenuItem *tools_insertDate = new wxMenuItem(menuTools, wxxInsertDate, "Insert &date", "");
    menuTools->Append(tools_insertDate);
    wxMenuItem *tools_insertTime = new wxMenuItem(menuTools, wxxInsertTime, "Insert &time", "");
    menuTools->Append(tools_insertTime);
    wxMenuItem *tools_insertDateTime = new wxMenuItem(menuTools, wxxInsertDateTime, "Insert date &and time", "");
    menuTools->Append(tools_insertDateTime);

    wxMenu *menuWindow = new wxMenu;
    for (int i=0; i<10; i++) {
        wxMenuItem *menuItem = new wxMenuItem(menuWindow, wxxID_Window+i, to_string(i), "");
        menuWindow->Append(menuItem);
        Bind(wxEVT_MENU, &MyFrame::OnWindow, this, wxxID_Window+i);
    }

    wxMenu *menuOther = new wxMenu;
    wxMenuItem *editas = new wxMenuItem(menuOther, wxxID_Editas, "Editas", "");
    menuOther->Append(editas);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuEdit, "&Edit");
    menuBar->Append(menuSearch, "&Search");
    menuBar->Append(menuTools, "&Tools");
    menuBar->Append(menuWindow, "&Window");
    menuBar->Append(menuOther, "&Other");

    SetMenuBar( menuBar );

    StartServer();
}

MyFrame::MyFrame(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, long style)
        : wxFrame(parent, id, title, pos, size, style) {
    CreateMenu();
    manager.SetManagedWindow(this);
    manager.SetFlags(wxAUI_MGR_DEFAULT);
    notebook = new wxAuiNotebook(this, ID_NOTEBOOK, wxDefaultPosition, wxDefaultSize, wxAUI_NB_TAB_SPLIT | wxAUI_NB_BOTTOM |
        wxAUI_NB_TAB_MOVE | wxAUI_NB_SCROLL_BUTTONS | wxAUI_NB_MIDDLE_CLICK_CLOSE| wxAUI_NB_CLOSE_BUTTON | wxAUI_NB_CLOSE_ON_ALL_TABS);
    wxAuiTabArt *art = new MyTabArt;
    notebook->SetArtProvider(art);
    Bind(wxEVT_AUINOTEBOOK_PAGE_CLOSE, &MyFrame::OnPageClose, this);

    manager.AddPane(notebook, wxAuiPaneInfo().Left().Caption(wxT("Edytor")).MaximizeButton(true).MinimizeButton(true).PinButton(true).PaneBorder(false).Dock().Resizable().FloatingSize(wxDefaultSize).CentrePane().DefaultPane());

    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(notebook, 1, wxEXPAND | wxALL, 0);
    this->SetSizer(mainSizer);
    CreateStatusBar();
    editorFactory = new EditorFactory(notebook);
    config = new Config();
    CmdLineOpenFiles();
    Bind(wxEVT_CLOSE_WINDOW, &MyFrame::OnCloseMain, this);
    Bind(wxEVT_CHAR_HOOK, &MyFrame::OnKeyDown, this);
}

void MyFrame::OnExit(wxCommandEvent &event) {
    Close(true);
}

wxStyledTextCtrl *MyFrame::SelectWord() {
    wxStyledTextCtrl *stc = editorFactory->GetCurrentWidget();
    if (!stc) return nullptr;
    int pos = stc->GetCurrentPos();
    int start = stc->WordStartPosition(pos, true);
    int end = stc->WordEndPosition(pos, true);
    stc->SetSelection(start, end);
    return stc;
}

void MyFrame::OnSelectWord(wxCommandEvent &event) {
    SelectWord();
}

void MyFrame::ChangeCase(ChangeCaseEnum cc) {
    wxStyledTextCtrl *stc = editorFactory->GetCurrentWidget();
    if (!stc) return;
    if (!stc->HasSelection())
        SelectWord();
    int cmd;
    if (cc==ccUpper) cmd = wxSTC_CMD_UPPERCASE;
    else cmd = wxSTC_CMD_LOWERCASE;
    stc->CmdKeyExecute(cmd);
}

void MyFrame::OnChangeCaseUpper(wxCommandEvent &event) {
    ChangeCase(ccUpper);
}

void MyFrame::OnChangeCaseLower(wxCommandEvent &event) {
    ChangeCase(ccLower);
}

void MyFrame::WordNextPrev(bool prev) {
    wxStyledTextCtrl *stc = editorFactory->GetCurrentWidget();
    if (!stc) return;
    int pos = stc->GetCurrentPos();
    int start = stc->WordStartPosition(pos, true);
    int end = stc->WordEndPosition(pos, true);
    stc->SetSelection(start, end);
    wxString word = stc->GetSelectedText();
    int flags = wxFR_WHOLEWORD;
    if (prev)
        pos = stc->FindText(start, 0, word, flags);
    else
        pos = stc->FindText(end, stc->GetTextLength(), word, flags);
    if (pos>=0) {
        stc->SetSelection(pos, pos);
        int endpos = stc->WordEndPosition(pos, true);
        stc->SetSelection(pos, endpos);
        stc->EnsureCaretVisible();

    }
    else
        stc->SetSelection(start, start);
}

void MyFrame::OnWordNext(wxCommandEvent &event) {
    WordNextPrev(false);
}

void MyFrame::OnWordPrev(wxCommandEvent &event) {
    WordNextPrev(true);
}

void MyFrame::OnEditas(wxCommandEvent &event) {
    vector<string> params;
    Editor *editor = editorFactory->GetCurrentEditor();
    if (editor)
        params.push_back(string(editor->GetPath().c_str()));
    execute("/home/andrzej/gitmy/editas-code/editas", params, false);
}

#define SUPPORTED_FILES_EXT wxT("Headers (*.h)|*.h|" \
        "Headers (*.hpp)|*.hpp|" \
        "Headers (*.hxx)|*.hxx|" \
        "Sources (*.c)|*.c|" \
        "Sources (*.cpp)|*.cpp|" \
        "Sources (*.cxx)|*.cxx|" \
        "All |*.*")

void MyFrame::OnNewPage(wxCommandEvent &event) {
    OpenInEditor("");
}

void MyFrame::OnOpenFile(wxCommandEvent &event) {
    wxFileDialog openDialog(this, wxT("Open file"), "..", "", SUPPORTED_FILES_EXT, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openDialog.ShowModal() == wxID_CANCEL) return;
    OpenOrActivate(openDialog.GetPath());
}

void MyFrame::OnReopenFile(wxCommandEvent &event) {
    Editor *editor = editorFactory->GetCurrentEditor();
    if (editor)
        editor->Reopen(false);
}

void MyFrame::OnSaveFile(wxCommandEvent &event) {
    Editor *editor = editorFactory->GetCurrentEditor();
    if (editor)
        editor->Save();
}

void MyFrame::OnSaveAs(wxCommandEvent &event) {
    Editor *editor = editorFactory->GetCurrentEditor();
    if (!editor) return;
    wxFileName file(editor->GetPath());
    wxString dir;
    file.DirName(dir);
    wxFileDialog saveDialog(this, wxT("Save as"), dir, editor->GetPath(), SUPPORTED_FILES_EXT, wxFD_SAVE);
    if (saveDialog.ShowModal() == wxID_CANCEL) return;
    editor->SaveAs(saveDialog.GetPath());
}

void MyFrame::OnSaveAll(wxCommandEvent &event) {
    int count = editorFactory->GetEditorCount();
    for (int i = 0; i < count; i++) {
        Editor* editor = editorFactory->GetEditor(i);
        ConsiderEnum consider = editor->Consider();
        if (consider==coCanSave) editor->Save();
    }
}

void MyFrame::OnClose(wxCommandEvent &event) {
    editorFactory->CloseCurrent();
}

void MyFrame::OnCloseAll(wxCommandEvent &event) {
    CloseAll();
}

void MyFrame::OnFind(wxCommandEvent& event)
{
    if (!editorFactory->GetCurrentEditor()) return;
    if (findDialog)
    {
        wxDELETE(findDialog);
        return;
    }
    wxString dialog_title = wxT("Find...");
    int dialog_style = 0;

    if(event.GetId() == wxID_REPLACE)
    {
        dialog_title = wxT("Find and replace...");
        dialog_style = wxFR_REPLACEDIALOG;
    }

    findData.SetFlags(wxFR_DOWN);
    findDialog = new wxFindReplaceDialog(this, &findData, dialog_title, dialog_style);
    findDialog->Show(true);
}


void MyFrame::OnDoFind(wxFindDialogEvent& event)
{
    wxStyledTextCtrl *stc = editorFactory->GetCurrentWidget();
    if (!stc) return;
    int result = DoFind(stc, event.GetFindString(), event.GetFlags());
    if(result == -1)
    {
        wxMessageBox(wxT("Reached end of file\nSearch from begin."));
        if (event.GetFlags() & wxFR_DOWN)
        {
            stc->DocumentStart();
        }
        else
        {
            stc->DocumentEnd();
        }
    }
    else
    {
        stc->EnsureCaretVisible();
    }
}

void MyFrame::OnDoFindReplace(wxFindDialogEvent& event)
{
    wxStyledTextCtrl *stc = editorFactory->GetCurrentWidget();
    if (!stc) return;
    if (event.GetEventType() == wxEVT_FIND_REPLACE)
    {
        if (!stc->GetSelections())
        {
            stc->ReplaceSelection(event.GetReplaceString());
        }
        else
        {
            wxLogMessage(wxT("No selection, search"));
        }
    }
    else // wxEVT_FIND_REPLACE_ALL
    {
        int count = 0;
        int result = -1;

        stc->DocumentStart();

        do
        {
            result = DoFind(stc, event.GetFindString(), event.GetFlags());
            if (result != -1)
            {
                stc->ReplaceSelection(event.GetReplaceString());
                count++;
            }
        } while (result != -1);
        wxLogMessage(wxString::Format("Number replaced: %d", count));
    }
}

void MyFrame::OnFindDialogClose(wxFindDialogEvent& WXUNUSED(event))
{
    findDialog->Destroy();
    findDialog = NULL;
}

int MyFrame::DoFind(wxStyledTextCtrl* stc, const wxString& str, int flags)
{
    // Ustawienie pozycji
    int pos = stc->GetCurrentPos();

    // Mapowanie flag wyszukiwania dla wxStyledTextCtrl
    int find_flags = 0;
    if (flags & wxFR_MATCHCASE) find_flags |= wxSTC_FIND_MATCHCASE;
    if (flags & wxFR_WHOLEWORD) find_flags |= wxSTC_FIND_WHOLEWORD;

    // Zmienna pomocnicza zmiany kierunków szukania
    static int last_direction = wxFR_DOWN;

    // WYSZUKIWANIE
    int result = -1;
    if (flags & wxFR_DOWN)
    {
        // Wyszukiwanie w dół
        if (last_direction != wxFR_DOWN)
        {
            pos += str.Len();
            stc->GotoPos(pos);
        }
        stc->SetAnchor(pos);
        stc->SearchAnchor();
        result = stc->SearchNext(find_flags, str);
        last_direction = wxFR_DOWN;
    }
    else
    {
        // Wyszukiwanie w górę
        stc->SearchAnchor();
        result = stc->SearchPrev(find_flags, str);
        last_direction = -1;
    }

    if (result != -1)
    {
        if (flags & wxFR_DOWN)
        {
            stc->SetSelection(result, stc->GetAnchor());
        }
    }
    return result;
}

void MyFrame::OnGoto(wxCommandEvent& event) {
    wxStyledTextCtrl *stc = editorFactory->GetCurrentWidget();
    if (!stc) return;
    int pos = stc->GetCurrentPos();
    wxTextEntryDialog dialog(this,
                             "Please enter position",
                             "Go to line:row",
                             wxString::Format("%d:%d",stc->GetCurrentLine()+1, stc->GetColumn(pos)+1),
                             wxOK | wxCANCEL);
    if (dialog.ShowModal() == wxID_OK)
    {
        wxString str = dialog.GetValue();
        int gotoLine, gotoCol;
        int posColon = str.Find(":");
        if (posColon==wxNOT_FOUND) {
            gotoCol = 1;
            gotoLine =  wxAtoi(str);
        }
        else {
            gotoCol = wxAtoi(str.SubString(posColon + 1, str.Length()));
            gotoLine =  wxAtoi(str.SubString(0, posColon));
        }
        if (gotoLine==0 || gotoCol==0) {
            wxMessageBox("Bad input string");
            return;
        }
        int pos = stc->PositionFromLine(gotoLine-1);
        pos += gotoCol-1;
        stc->SetCurrentPos(pos);
        stc->SetSelection(pos,pos);
        stc->EnsureCaretVisible();
    }
}

void MyFrame::OnInsertDate(wxCommandEvent &event) {
    Editor *editor = editorFactory->GetCurrentEditor();
    if (editor)
        editor->InsertDate();
}

void MyFrame::OnInsertTime(wxCommandEvent &event) {
    Editor *editor = editorFactory->GetCurrentEditor();
    if (editor)
        editor->InsertTime();
}

void MyFrame::OnInsertDateTime(wxCommandEvent &event) {
    Editor *editor = editorFactory->GetCurrentEditor();
    if (editor)
        editor->InsertDateTime();
}

void MyFrame::OpenInEditor(const wxString &file_path) {
    Editor *editor = editorFactory->CreateTabSheet(file_path);
    Bind(wxEVT_STC_MARGINCLICK, &MyFrame::OnStcMarginClick, this, editor->GetId());
    Bind(wxEVT_STC_MODIFIED, &MyFrame::OnStcModified, this, editor->GetId());
}

void MyFrame::OpenOrActivate(const wxString& file_path) {
    Editor *editor = editorFactory->GetEditorByPath(file_path);
    if (editor) {
        editor->Reopen(false);
        editor->Activate();
        editor->GetWidget()->SetFocus();
    }
    else
        OpenInEditor(file_path);
}

void MyFrame::OnStcMarginClick(wxStyledTextEvent &event) {
    wxStyledTextCtrl* stc = static_cast<wxStyledTextCtrl*>(event.GetEventObject());

    switch (event.GetMargin())
    {
        case STC_LINE_NUM_MARGIN:
            break;

            // Obsługa marginesu znaczników...
        case STC_FOLD_MARGIN:
        {
            int line_number = stc->LineFromPosition(event.GetPosition());
            if (stc->GetFoldLevel(line_number) & wxSTC_FOLDLEVELHEADERFLAG)
            {
                stc->ToggleFold(line_number);
            }
            break;
        }

        default:
            break;
    }
}

void MyFrame::OnStcModified(wxStyledTextEvent& event) {
    notebook->Refresh();
}

void MyFrame::OnPageClose(wxAuiNotebookEvent &event) {
    CloseEnum closeEnum = clClose;
    editorFactory->CloseEditor(event.GetSelection(), false, closeEnum);
    if (closeEnum==clCancel)
        event.Veto();
}

void MyFrame::CmdLineOpenFiles() {
    wxApp &app = wxGetApp();
    for (int i=1; i<app.argc; i++)
        OpenOrActivate(app.argv[i]);
}

bool MyFrame::CloseAll() {
    int count = editorFactory->GetEditorCount();
    int considerCnt = 0;
    for (int i = count - 1; i >= 0; i--) {
        ConsiderEnum consider = editorFactory->GetEditor(i)->Consider();
        if (consider!=coCanClose)considerCnt++;
    }
    CloseEnum closeEnum = clClose;
    for (int i = count - 1; i >= 0; i--) {
        editorFactory->CloseEditor(i,considerCnt>1, closeEnum);
        if (closeEnum!=clCancel) {
            wxTheClipboard->Flush();
            notebook->DeletePage(i);
        }
        else {
            return false;
        }
    }
    return true;
}

void MyFrame::OnCloseMain(wxCloseEvent& event)
{
    if (CloseAll()) {
        wxTheClipboard->Flush();
        wxDELETE(m_server);
        Destroy();
    }
    else
        event.Veto();
}

void MyFrame::OnKeyDown(wxKeyEvent &event) {
    int code = event.m_keyCode;
    if (code >= '0' && code <= '9' && event.AltDown()
                    && !event.ControlDown()  && !event.ShiftDown()) {
        int n;
        if (code=='0') n=9;
        else n=code-'1';
        int count = editorFactory->GetEditorCount();
        if (n<count)
        {
            Editor *editor = editorFactory->GetEditor(n);
            editor->Activate();
        }
    }
    else event.Skip();
}

void MyFrame::OnCut(wxCommandEvent &event) {
    wxStyledTextCtrl *stc = editorFactory->GetCurrentWidget();
    if (!stc) return;
    stc->CmdKeyExecute(wxSTC_CMD_CUT);
}

void MyFrame::OnCopy(wxCommandEvent &event) {
    wxStyledTextCtrl *stc = editorFactory->GetCurrentWidget();
    if (!stc) return;
    stc->CmdKeyExecute(wxSTC_CMD_COPY);
}

void MyFrame::OnPaste(wxCommandEvent &event) {
    wxStyledTextCtrl *stc = editorFactory->GetCurrentWidget();
    if (!stc) return;
    stc->CmdKeyExecute(wxSTC_CMD_PASTE);
}

void MyFrame::OnDelete(wxCommandEvent &event) {
    wxStyledTextCtrl *stc = editorFactory->GetCurrentWidget();
    if (!stc) return;
    stc->CmdKeyExecute(wxSTC_CMD_CLEAR);
}

void MyFrame::OnSelectAll(wxCommandEvent &event) {
    wxStyledTextCtrl *stc = editorFactory->GetCurrentWidget();
    if (!stc) return;
    stc->CmdKeyExecute(wxSTC_CMD_SELECTALL);
}

void MyFrame::OnWindow(wxCommandEvent& event)
{
    wxMessageBox(to_string(event.GetId()), "caption");
}
