#include "main.h"
#include <wx/event.h>
#include <wx/filename.h>
#include <string>
#include <vector>
#include "libipc/ipc.h"
#include "execute.h"

using namespace std;

wxIMPLEMENT_APP(MyApp);
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
                EVT_MENU(wxID_OPEN, MyFrame::OnOpenFile)
                EVT_MENU(wxxInsertDate, MyFrame::OnInsertDate)
                EVT_MENU(wxxInsertTime, MyFrame::OnInsertTime)
                EVT_MENU(wxxInsertDateTime, MyFrame::OnInsertDateTime)
END_EVENT_TABLE()

ipc::channel sender__   { "Scintas.IPC", ipc::sender   };
ipc::channel receiver__ { "Scintas.IPC", ipc::receiver };

bool MyApp::OnInit() {
    //ipc::buff_t buf = receiver__.recv();
    m_checker = new wxSingleInstanceChecker;
    if ( m_checker->IsAnotherRunning() )
    {
        wxApp &app = wxGetApp();
        for (int i=1; i<app.argc; i++)
            sender__.send(std::string(app.argv[i].c_str()));
        delete m_checker; // OnExit() won't be called if we return false
        m_checker = NULL;
        return false;
    }
    MyFrame *frame = new MyFrame(NULL, wxID_ANY, "Scintas", wxDefaultPosition, wxSize(800,600), wxDEFAULT_FRAME_STYLE);;
    frame->Show(true);
    return true;
}

int MyApp::OnExit()
{
    delete m_checker;
    return 0;
}

void MyFrame::CreateMenu() {
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(wxID_NEW);
    wxMenuItem *open_file = new wxMenuItem(menuFile, wxID_OPEN, "Open file\tCtrl-O", "");
    menuFile->Append(open_file);
    wxMenuItem *recent_file = new wxMenuItem(menuFile, wxID_OPEN, "&Recent files", "");
    menuFile->Append(recent_file);
    wxMenuItem *file_according_name = new wxMenuItem(menuFile, wxID_OPEN, "According to name", "");
    menuFile->Append(file_according_name);
    wxMenuItem *file_according_path = new wxMenuItem(menuFile, wxID_OPEN, "According to path", "");
    menuFile->Append(file_according_path);
    wxMenuItem *file_reopen = new wxMenuItem(menuFile, wxID_OPEN, "Reopen file", "");
    menuFile->Append(file_reopen);
    menuFile->Append(wxID_SEPARATOR);
    menuFile->Append(wxID_SAVE);
    menuFile->Append(wxID_SAVEAS);
    wxMenuItem *file_saveall = new wxMenuItem(menuFile, wxID_OPEN, "Save all\tCtrl-Shift-S", "");
    menuFile->Append(file_saveall);
    menuFile->Append(wxID_CLOSE);
    wxMenuItem *file_closeall = new wxMenuItem(menuFile, wxID_OPEN, "Close all\tCtrl-Shift-F4", "");
    menuFile->Append(file_closeall);
    wxMenuItem *exit_file = new wxMenuItem(menuFile, wxID_EXIT, "E&xit\tAlt-F4", "");
    menuFile->Append(exit_file);

    wxMenu *menuEdit = new wxMenu;
    menuEdit->Append(wxID_UNDO);
    menuEdit->Append(wxID_REDO);
    menuEdit->Append(wxID_SEPARATOR);
    wxMenuItem *edit_cut = new wxMenuItem(menuFile, wxID_OPEN, "Cut\tCtrl-X", "");
    menuEdit->Append(edit_cut);
    wxMenuItem *edit_copy = new wxMenuItem(menuFile, wxID_OPEN, "Copy\tCtrl-C", "");
    menuEdit->Append(edit_copy);
    wxMenuItem *edit_paste = new wxMenuItem(menuFile, wxID_OPEN, "Paste\tCtrl-V", "");
    menuEdit->Append(edit_paste);
    wxMenuItem *edit_delete = new wxMenuItem(menuFile, wxID_OPEN, "Delete", "");
    menuEdit->Append(edit_delete);
    wxMenuItem *edit_selectAll = new wxMenuItem(menuFile, wxID_OPEN, "Select all\tCtrl+A", "");
    menuEdit->Append(edit_selectAll);
    wxMenuItem *edit_selectWord = new wxMenuItem(menuFile, wxID_OPEN, "Select word\tCtrl+W", "");
    menuEdit->Append(edit_selectWord);
    wxMenuItem *edit_selectMode = new wxMenuItem(menuFile, wxID_OPEN, "Select mode", "");
    menuEdit->Append(edit_selectMode);
    wxMenuItem *edit_changeCase = new wxMenuItem(menuFile, wxID_OPEN, "Change case", "");
    menuEdit->Append(edit_changeCase);
    new wxMenuItem(menuFile, wxID_OPEN, "Block indent\tTAb", "");
    menuEdit->Append(wxID_SEPARATOR);
    wxMenuItem *edit_blockIndent = new wxMenuItem(menuFile, wxID_OPEN, "Block indent\tTab", "");
    menuEdit->Append(edit_blockIndent);
    wxMenuItem *edit_blockUnindent = new wxMenuItem(menuFile, wxID_OPEN, "Block unindent\tShift+Tab", "");
    menuEdit->Append(edit_blockUnindent);

    wxMenu *menuSearch = new wxMenu;
    menuSearch->Append(wxID_FIND);
    wxMenuItem *search_findNext = new wxMenuItem(menuSearch, wxID_OPEN, "Find next", "");
    menuEdit->Append(search_findNext);

    wxMenu *menuTools = new wxMenu;
    wxMenuItem *tools_insertDate = new wxMenuItem(menuTools, wxxInsertDate, "Insert &date", "");
    menuTools->Append(tools_insertDate);
    wxMenuItem *tools_insertTime = new wxMenuItem(menuTools, wxxInsertTime, "Insert &time", "");
    menuTools->Append(tools_insertTime);
    wxMenuItem *tools_insertDateTime = new wxMenuItem(menuTools, wxxInsertDateTime, "Insert date &and time", "");
    menuTools->Append(tools_insertDateTime);

    wxMenu *menuOther = new wxMenu;
    wxMenuItem *editas = new wxMenuItem(menuOther, wxxID_Editas, "Editas", "");
    menuOther->Append(editas);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuEdit, "&Edit");
    menuBar->Append(menuSearch, "&Search");
    menuBar->Append(menuTools, "&Tools");
    menuBar->Append(menuOther, "&Other");

    SetMenuBar( menuBar );
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &MyFrame::OnEditas, this, wxxID_Editas);
}

MyFrame::MyFrame(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, long style)
        : wxFrame(parent, id, title, pos, size, style) {
    CreateMenu();
    manager.SetManagedWindow(this);
    manager.SetFlags(wxAUI_MGR_DEFAULT);
    notebook = new wxAuiNotebook(this, ID_NOTEBOOK, wxDefaultPosition, wxDefaultSize, wxAUI_NB_TAB_SPLIT | wxAUI_NB_BOTTOM |
                                                                                      wxAUI_NB_TAB_MOVE | wxAUI_NB_SCROLL_BUTTONS | wxAUI_NB_MIDDLE_CLICK_CLOSE| wxAUI_NB_CLOSE_BUTTON | wxAUI_NB_CLOSE_ON_ALL_TABS);
    manager.AddPane(notebook, wxAuiPaneInfo().Left().Caption(wxT("Edytor")).MaximizeButton(true).MinimizeButton(true).PinButton(true).PaneBorder(false).Dock().Resizable().FloatingSize(wxDefaultSize).CentrePane().DefaultPane());

    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(notebook, 1, wxEXPAND | wxALL, 0);
    this->SetSizer(mainSizer);
    CreateStatusBar();
    editorFactory = new EditorFactory(notebook);
    config = new Config();
    CmdLineOpenFiles();
    instanceTimer.Bind(wxEVT_TIMER, &MyFrame::OnInstanceTimer, this);
    instanceTimer.Start(200);
}

void MyFrame::OnInstanceTimer(wxTimerEvent&)
{
    ipc::buffer buf = receiver__.try_recv();
    while (!buf.empty()) {
        std::string param {buf.get < char const *>(), buf.size() - 1 };
        OpenOrActivate(param);
        buf = receiver__.try_recv();
    }
}

void MyFrame::OnExit(wxCommandEvent &event) {
    Close(true);
}

void MyFrame::OnEditas(wxCommandEvent &event) {
    vector<string> params;
    execute("/home/andrzej/gitmy/editas-code/editas", params, false);
}

#define SUPPORTED_FILES_EXT wxT("Headers (*.h)|*.h|" \
        "Headers (*.hpp)|*.hpp|" \
        "Headers (*.hxx)|*.hxx|" \
        "Sources (*.c)|*.c|" \
        "Sources (*.cpp)|*.cpp|" \
        "Sources (*.cxx)|*.cxx|" \
        "All |*.*")

void MyFrame::OnOpenFile(wxCommandEvent &event) {
    wxFileDialog openDialog(this, wxT("Open file"), "..", "", SUPPORTED_FILES_EXT, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openDialog.ShowModal() == wxID_CANCEL) return;
    OpenOrActivate(openDialog.GetPath());
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
}

void MyFrame::OpenOrActivate(const wxString& file_path) {
    Editor *editor = editorFactory->GetEditorByPath(file_path);
    if (editor)
        editor->Activate();
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

void MyFrame::CmdLineOpenFiles() {
    wxApp &app = wxGetApp();
    for (int i=1; i<app.argc; i++)
        OpenOrActivate(app.argv[i]);
}
