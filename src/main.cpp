#include "main.h"
#include <wx/event.h>
#include <wx/filename.h>

wxIMPLEMENT_APP(MyApp);

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
                EVT_MENU(wxID_OPEN, MyFrame::OnOpenFile)
END_EVENT_TABLE()

bool MyApp::OnInit() {
    MyFrame *frame = new MyFrame(NULL, wxID_ANY, "Scintas", wxDefaultPosition, wxSize(800,600), wxDEFAULT_FRAME_STYLE);;
    frame->Show(true);
    return true;
}

MyFrame::MyFrame(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, long style)
        : wxFrame(parent, id, title, pos, size, style) {
    wxMenu *menuFile = new wxMenu;
    wxMenuItem *open_file = new wxMenuItem(menuFile, wxID_OPEN, wxT("Open file\tCtrl-O"), "");
    menuFile->Append(open_file);
    menuFile->Append(wxID_EXIT);
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    SetMenuBar( menuBar );
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);

    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(mainSizer);
    panel = new wxPanel(this);
    wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);
    panel->SetSizer(panelSizer);
    mainSizer->Add(panel, 1, wxEXPAND | wxALL, 0);
    splitter = new wxSplitterWindow(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3DSASH | wxSP_NO_XP_THEME | wxSP_LIVE_UPDATE);
    panelSizer->Add(splitter, 1, wxEXPAND | wxALL, 0);

    left = new wxPanel(splitter, wxID_ANY);
    right = new wxPanel(splitter, wxID_ANY);

    left->SetMinSize(wxSize(200, -1));
    right->SetMinSize(wxSize(200, -1));

    splitter->SplitVertically(left, right, 250);

    wxBoxSizer *rightSizer = new wxBoxSizer(wxVERTICAL);
    right->SetSizer(rightSizer);

    rightTop = new wxPanel(right, wxID_ANY);
    statuBar = new wxStatusBar(right, wxID_ANY);

    rightSizer->Add(rightTop, 1, wxEXPAND | wxALL, 0);
    rightSizer->Add(statuBar);

    rightTop->SetMinSize(wxSize(-1, 200));

    wxBoxSizer* left_sizer = new wxBoxSizer(wxVERTICAL);
    left->SetSizer(left_sizer);

    wxBoxSizer *rightTop_sizer = new wxBoxSizer(wxVERTICAL);
    rightTop->SetSizer(rightTop_sizer);

    wxBoxSizer *rightBottom_sizer = new wxBoxSizer(wxVERTICAL);
    statuBar->SetSizer(rightBottom_sizer);

    manager.SetManagedWindow(this);
    manager.SetFlags(wxAUI_MGR_DEFAULT);

    notebook = new wxAuiNotebook(rightTop, ID_NOTEBOOK, wxDefaultPosition, wxDefaultSize, wxAUI_NB_TAB_SPLIT | wxAUI_NB_BOTTOM |
            wxAUI_NB_TAB_MOVE | wxAUI_NB_SCROLL_BUTTONS | wxAUI_NB_MIDDLE_CLICK_CLOSE| wxAUI_NB_CLOSE_BUTTON | wxAUI_NB_CLOSE_ON_ALL_TABS);

    manager.AddPane(notebook, wxAuiPaneInfo().Left().Caption(wxT("Edytor")).MaximizeButton(true).MinimizeButton(true).PinButton(true).PaneBorder(false).Dock().Resizable().FloatingSize(wxDefaultSize).CentrePane().DefaultPane());
    rightTop_sizer->Add(notebook, 1, wxEXPAND | wxALL, 0);

    editorFactory = new EditorFactory(notebook);
}

void MyFrame::OnExit(wxCommandEvent &event) {
    Close(true);
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
    OpenInEditor(openDialog.GetPath());
}

wxStyledTextCtrl *MyFrame::OpenInEditor(const wxString &file_path) {
    Editor *editor = editorFactory->CreateTabSheet(file_path);
    wxStyledTextCtrl* stc = editor->GetWidget();
    Bind(wxEVT_STC_MARGINCLICK, &MyFrame::OnStcMarginClick, this, editor->GetId());
    return stc;
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
