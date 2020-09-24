#include "Editor.h"
#include <wx/filename.h>
#include <chrono>
#include <time.h>

using namespace std;

Editor::Editor(wxStyledTextCtrl* stc, wxWindowID id, wxAuiNotebook* notebook)
            :stc(stc),id(id),notebook(notebook) {}

void Editor::Activate() {
    int page = notebook->GetPageIndex(stc);
    notebook->SetSelection(page);
}

void Editor::ActionsBeforeClose() {}
void Editor::AskSaveChangesBeforeClosing(CloseEnum &CanClose) {}
CloseEnum Editor::AskSaveChangesBeforeReopen() { return clClose;}
bool Editor::CanClose() { return true; }
void Editor::TryClose() {  }
wxStyledTextCtrl* Editor::GetWidget() { return stc; };
//function Editor::GetCaretPos: TPoint;
//function Editor::GetStateStr: string;
wxString Editor::GetPath() {
    return path;
}
wxString Editor::GetTitle() {
    return title;
}
//Editor::GetLines: TStrings;() override;
//Editor::GetCurrentLine: string;() override;
//Editor::LogicalToPhysicalCol(const Line: String; Index, LogicalPos: integer): integer;() override;
//bool Editor::GetModified() {  }
ConsiderEnum Editor::Consider() { return coCanClose;}

void Editor::SetEditorStyle() {
    stc->StyleClearAll();

    //line numbering
    stc->SetMarginWidth(0, 30);
    stc->SetMarginType(0, wxSTC_MARGIN_NUMBER);
    stc->StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColour(155, 255, 255));
    stc->StyleSetBackground(wxSTC_STYLE_LINENUMBER, wxColour(0, 128, 255));

    stc->SetWrapMode(0);

    stc->SetLexer(wxSTC_LEX_CPP);

    wxFont font(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Courier New");
    wxFont fontb(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Courier New");

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

void Editor::OpenFile(const wxString path) {
    this->path = path;
    wxFileName file(path);
    this->title = file.GetFullName();
    if (stc->LoadFile(path))
    {
        //Bind(wxEVT_STC_MARGINCLICK, &MyFrame::OnStcMarginClick, this, new_id);
        //Bind(wxEVT_STC_MODIFIED, &MyFrame::OnStcModified, this, new_id);
        //
    }
    else
    {
        wxLogWarning(wxT("Can't load ") + path + wxT("!"));
    }
    SetEditorStyle();
    stc->SetMarginWidth(0, 30);
    stc->SetMarginType(0, wxSTC_MARGIN_NUMBER);
    stc->SetMarginCursor(0,0);
    stc->SetMarginCursor(1,8);
    int maxPos=1000;
    stc->IndicatorClearRange(0, maxPos);
    stc->IndicatorSetStyle(1, wxSTC_INDIC_ROUNDBOX);
    stc->IndicatorSetAlpha(1, 100);
    stc->IndicatorSetUnder(1, true);
    stc->IndicatorSetForeground(1, wxColor(0, 255, 0));
    stc->SetIndicatorCurrent(1);
    stc->IndicatorFillRange(100, 20);
}
void Editor::Reopen(bool AskWhenModified) {}
FileChangeKind Editor::ChangedOutside() {return fckNothing;}
void Editor::SpecialChars(bool Show) {}

const static char* daysOfWeek[] = {
        "Sunday",
        "Monday",
        "Tuesday",
        "Wednesday",
        "Thursday",
        "Friday",
        "Saturday"
};

void Editor::InsertDate() {
    auto nowChrono = chrono::system_clock::now();
    time_t now_time_t = chrono::system_clock::to_time_t(nowChrono);
    struct tm* now_tm = localtime(&now_time_t);
    char buf[64];
    sprintf(buf,"%s %d-%.2d-%.2d",
            daysOfWeek[now_tm->tm_wday],
            now_tm->tm_year+1900, now_tm->tm_mon+1, now_tm->tm_mday);
    string str = buf;
    stc->InsertText(stc->GetCurrentPos(), str);
    stc->SetCurrentPos(stc->GetCurrentPos()+str.length());
}

void Editor::InsertTime() {
    auto nowChrono = chrono::system_clock::now();
    time_t now_time_t = chrono::system_clock::to_time_t(nowChrono);
    struct tm* now_tm = localtime(&now_time_t);
    char buf[64];
    sprintf(buf,"%.2d:%.2d:%.2d",
            now_tm->tm_hour, now_tm->tm_min, now_tm->tm_sec);
    string str = buf;
    stc->InsertText(stc->GetCurrentPos(), str);
    stc->SetCurrentPos(stc->GetCurrentPos()+str.length());
}

void Editor::InsertDateTime() {
    auto nowChrono = chrono::system_clock::now();
    time_t now_time_t = chrono::system_clock::to_time_t(nowChrono);
    struct tm* now_tm = localtime(&now_time_t);
    char buf[64];
    sprintf(buf,"%s %d-%.2d-%.2d %.2d:%.2d:%.2d",
            daysOfWeek[now_tm->tm_wday],
            now_tm->tm_year+1900, now_tm->tm_mon+1, now_tm->tm_mday,
            now_tm->tm_hour, now_tm->tm_min, now_tm->tm_sec);
    string str = buf;
    stc->InsertText(stc->GetCurrentPos(), str);
    stc->SetCurrentPos(stc->GetCurrentPos()+str.length());
}

void Editor::Save() {
    stc->SaveFile(path);
}

void Editor::SaveAs(const wxString newpath) {
    path = newpath;
    wxFileName file(path);
    title = file.GetFullName();
    stc->SaveFile(path);
}

bool Editor::IsModified() {
    return stc->IsModified();
}

//Editor::GetHighLighter: TSynCustomHighlighter;() {}
//Editor::SetHighLighter(Value: TSynCustomHighlighter);() {}
//Editor::GetActivateTime: TDateTime;() {}
