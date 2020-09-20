#include "main.h"
#include <wx/event.h>
#include <wx/filename.h>

wxIMPLEMENT_APP(MyApp);

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
                EVT_MENU(wxID_OPEN, MyFrame::OnOpenFile)
END_EVENT_TABLE()

bool MyApp::OnInit() {
    MyFrame *frame = new MyFrame(NULL, wxID_ANY, "CCat", wxDefaultPosition, wxSize(800,600), wxDEFAULT_FRAME_STYLE);;
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
    rightBottom = new wxPanel(right, wxID_ANY);

    rightSizer->Add(rightTop, 1, wxEXPAND | wxALL, 0);
    rightSizer->Add(rightBottom, 1, wxEXPAND | wxALL, 0);

    rightTop->SetMinSize(wxSize(-1, 200));
    rightBottom->SetMinSize(wxSize(-1, 100));

    wxBoxSizer* left_sizer = new wxBoxSizer(wxVERTICAL);
    left->SetSizer(left_sizer);

    wxBoxSizer *rightTop_sizer = new wxBoxSizer(wxVERTICAL);
    rightTop->SetSizer(rightTop_sizer);

    wxBoxSizer *rightBottom_sizer = new wxBoxSizer(wxVERTICAL);
    rightBottom->SetSizer(rightBottom_sizer);

    manager.SetManagedWindow(this);
    manager.SetFlags(wxAUI_MGR_DEFAULT);
    notebook = new wxAuiNotebook(rightTop, ID_NOTEBOOK, wxDefaultPosition, wxDefaultSize, wxAUI_NB_TAB_SPLIT |
                                                                                          wxAUI_NB_DEFAULT_STYLE | wxAUI_NB_DEFAULT_STYLE | wxAUI_NB_CLOSE_BUTTON | wxAUI_NB_CLOSE_ON_ALL_TABS);
    manager.AddPane(notebook, wxAuiPaneInfo().Left().Caption(wxT("Edytor")).MaximizeButton(true).MinimizeButton(true).PinButton(true).PaneBorder(false).Dock().Resizable().FloatingSize(wxDefaultSize).CentrePane().DefaultPane());
    rightTop_sizer->Add(notebook, 1, wxEXPAND | wxALL, 0);
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

static long GetNewEditorId()
{
    static int id = 10000;
    return id++;
}

wxStyledTextCtrl *MyFrame::OpenInEditor(const wxString &file_path) {
    wxWindowID new_id = GetNewEditorId();
    wxStyledTextCtrl* stc = new wxStyledTextCtrl(notebook, new_id, wxDefaultPosition, wxDefaultSize, 0L, file_path);
    SetEditorStyle(stc);
    Bind(wxEVT_STC_MARGINCLICK, &MyFrame::OnStcMarginClick, this, new_id);
    //stc->SetKeyWords(0, wxGetWXKeyWords());
    //stc->SetKeyWords(1, wxGetCPP11KeyWords());
    if (stc->LoadFile(file_path))
    {
        //Bind(wxEVT_STC_MARGINCLICK, &MyFrame::OnStcMarginClick, this, new_id);
        //Bind(wxEVT_STC_MODIFIED, &MyFrame::OnStcModified, this, new_id);
        wxFileName file(file_path);
        notebook->AddPage(stc, file.GetFullName(), true);
    }
    else
    {
        wxLogWarning(wxT("Can't load ") + file_path + wxT("!"));
    }
    int maxPos=1000;
    stc->SetMarginWidth(0, 30);
    stc->SetMarginType(0, wxSTC_MARGIN_NUMBER);
    stc->SetMarginCursor(0,0);
    stc->SetMarginCursor(1,8);
    stc->IndicatorClearRange(0, maxPos);
    stc->IndicatorSetStyle(1, wxSTC_INDIC_ROUNDBOX);
    stc->IndicatorSetAlpha(1, 100);
    stc->IndicatorSetUnder(1, true);
    stc->IndicatorSetForeground(1, wxColor(0, 255, 0));
    stc->SetIndicatorCurrent(1);
    stc->IndicatorFillRange(100, 20);
    return stc;
}

void MyFrame::SetEditorStyle(wxStyledTextCtrl *stc) {
    stc->StyleClearAll();

    //line numbering
    stc->SetMarginWidth(0, 30);
    stc->SetMarginType(0, wxSTC_MARGIN_NUMBER);
    stc->StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColour(155, 255, 255));
    stc->StyleSetBackground(wxSTC_STYLE_LINENUMBER, wxColour(0, 128, 255));

    stc->SetWrapMode(0);

    stc->SetLexer(wxSTC_LEX_CPP);

    wxFont font(10, wxSWISS, wxNORMAL, wxNORMAL, false, "Courier New");
    wxFont fontb(10, wxSWISS, wxNORMAL, wxFONTWEIGHT_BOLD, false, "Courier New");

    stc->StyleSetFont(wxSTC_C_DEFAULT, font);
    stc->StyleSetFont(wxSTC_C_COMMENT, font);
    stc->StyleSetFont(wxSTC_C_COMMENTLINE, font);
    stc->StyleSetFont(wxSTC_C_COMMENTDOC, font);
    stc->StyleSetFont(wxSTC_C_NUMBER, font);
    stc->StyleSetFont(wxSTC_C_WORD, fontb);
    stc->StyleSetFont(wxSTC_C_STRING, font);
    stc->StyleSetFont(wxSTC_C_CHARACTER, font);
    stc->StyleSetFont(wxSTC_C_UUID, font);
    stc->StyleSetFont(wxSTC_C_PREPROCESSOR, font);
    stc->StyleSetFont(wxSTC_C_OPERATOR, font);
    stc->StyleSetFont(wxSTC_C_IDENTIFIER, font);
    stc->StyleSetFont(wxSTC_C_STRINGEOL, font);
    stc->StyleSetFont(wxSTC_C_VERBATIM, font);
    stc->StyleSetFont(wxSTC_C_REGEX, font);
    stc->StyleSetFont(wxSTC_C_COMMENTLINEDOC, font);
    stc->StyleSetFont(wxSTC_C_WORD2, fontb);
    stc->StyleSetFont(wxSTC_C_COMMENTDOCKEYWORD, font);
    stc->StyleSetFont(wxSTC_C_COMMENTDOCKEYWORDERROR, font);
    stc->StyleSetFont(wxSTC_C_GLOBALCLASS, font);
    stc->StyleSetFont(wxSTC_C_STRINGRAW, font);
    stc->StyleSetFont(wxSTC_C_TRIPLEVERBATIM, font);
    stc->StyleSetFont(wxSTC_C_HASHQUOTEDSTRING, font);
    stc->StyleSetFont(wxSTC_C_PREPROCESSORCOMMENT, font);

    stc->StyleSetForeground(wxSTC_C_STRING, wxColour(255, 0, 0));
    stc->StyleSetForeground(wxSTC_C_PREPROCESSOR, wxColour(0, 128, 0));
    stc->StyleSetForeground(wxSTC_C_IDENTIFIER, wxColour(0, 0, 0));
    stc->StyleSetForeground(wxSTC_C_NUMBER, wxColour(128, 0, 128));
    stc->StyleSetForeground(wxSTC_C_CHARACTER, wxColour(150, 0, 0));
    stc->StyleSetForeground(wxSTC_C_WORD, wxColour(0, 64, 128));
    stc->StyleSetForeground(wxSTC_C_WORD2, wxColour(0, 128, 128));
    stc->StyleSetForeground(wxSTC_C_COMMENT, wxColour(0, 128, 192));
    stc->StyleSetForeground(wxSTC_C_COMMENTLINE, wxColour(0, 128, 192));
    stc->StyleSetForeground(wxSTC_C_COMMENTDOC, wxColour(0, 128, 192));
    stc->StyleSetForeground(wxSTC_C_COMMENTDOCKEYWORD, wxColour(0, 128, 192));
    stc->StyleSetForeground(wxSTC_C_COMMENTDOCKEYWORDERROR, wxColour(0, 128, 192));

    stc->StyleSetBold(wxSTC_C_WORD, true);
    stc->StyleSetBold(wxSTC_C_WORD2, true);
    stc->StyleSetBold(wxSTC_C_COMMENTDOCKEYWORD, true);

    // code asemble
    stc->SetMarginWidth(1, 16);
    stc->SetMarginType(1, wxSTC_MARGIN_SYMBOL);
    stc->SetMarginMask(1, wxSTC_MASK_FOLDERS);
    stc->StyleSetBackground(1, wxColor(200, 200, 200));
    stc->SetMarginSensitive(1, true);

    stc->SetProperty(wxT("fold"), wxT("1"));

    wxColour custom(100, 100, 100);
    // folded main block
    stc->MarkerDefine(wxSTC_MARKNUM_FOLDER, wxSTC_MARK_BOXPLUS);
    stc->MarkerSetForeground(wxSTC_MARKNUM_FOLDER, *wxWHITE);
    stc->MarkerSetBackground(wxSTC_MARKNUM_FOLDER, custom);

    // unfolded main block
    stc->MarkerDefine(wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_BOXMINUS);
    stc->MarkerSetForeground(wxSTC_MARKNUM_FOLDEROPEN, *wxWHITE);
    stc->MarkerSetBackground(wxSTC_MARKNUM_FOLDEROPEN, custom);

    // end main block
    stc->MarkerDefine(wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_LCORNER);
    stc->MarkerSetBackground(wxSTC_MARKNUM_FOLDERTAIL, custom);

    // folded inner block
    stc->MarkerDefine(wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_BOXPLUSCONNECTED);
    stc->MarkerSetForeground(wxSTC_MARKNUM_FOLDEREND, *wxWHITE);
    stc->MarkerSetBackground(wxSTC_MARKNUM_FOLDEREND, custom);

    // unfolded inner block
    stc->MarkerDefine(wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_BOXMINUSCONNECTED);
    stc->MarkerSetForeground(wxSTC_MARKNUM_FOLDEROPENMID, *wxWHITE);
    stc->MarkerSetBackground(wxSTC_MARKNUM_FOLDEROPENMID, custom);

    // end inner block
    stc->MarkerDefine(wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_TCORNER);
    stc->MarkerSetBackground(wxSTC_MARKNUM_FOLDERMIDTAIL, custom);

    // block content
    stc->MarkerDefine(wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_VLINE);
    stc->MarkerSetBackground(wxSTC_MARKNUM_FOLDERSUB, custom);
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
