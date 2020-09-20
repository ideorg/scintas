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

const int menuID_Editas  = wxID_HIGHEST+1;

void MyFrame::CreateMenu() {
    wxMenu *menuFile = new wxMenu;
    wxMenuItem *open_file = new wxMenuItem(menuFile, wxID_OPEN, wxT("Open file\tCtrl-O"), "");
    menuFile->Append(open_file);
    menuFile->Append(wxID_EXIT);

    wxMenu *menuOther = new wxMenu;
    wxMenuItem *editas = new wxMenuItem(menuOther, menuID_Editas, "Editas", "");
    menuOther->Append(editas);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuOther, "&Other");

    SetMenuBar( menuBar );
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &MyFrame::OnEditas, this, menuID_Editas);
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
