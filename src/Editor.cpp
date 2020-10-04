#include "Editor.h"
#include <wx/filename.h>
#include <chrono>
#include <time.h>
#include "MessageBox/MessageBox.h"

using namespace std;

Editor::Editor(wxStyledTextCtrl* stc, wxWindowID id, wxAuiNotebook* notebook)
            :stc(stc),id(id),notebook(notebook) {}

void Editor::Activate() {
    int page = notebook->GetPageIndex(stc);
    notebook->SetSelection(page);
}

bool Editor::IsEmpty() {
    return stc->IsEmpty();
}

void Editor::ActionsBeforeClose() {}

void Editor::AskSaveChangesBeforeClosing(CloseEnum &CanClose, bool extended) {
     if (IsEmpty()) {
         CanClose = clClose;
         return;
     }
    if (CanClose == clCloseAllDiscard) return;
    if (CanClose == clCloseAllSave) {
        if (!Save()) {
            CanClose = clCancel;
            return;
        }
    }
    unsigned flags = wxYES | wxNO | wxCANCEL;
    if (extended) flags |= wxYES_TO_ALL | wxNO_TO_ALL;
    wxString message = "The text in the \"%s\" file has been changed.\n\n";
    message += "Do you want to save the modifications? (No = close and discard changes)";
    message = message.Format(message,GetTitle());
    unsigned modalResult = wxxMessageBox(message , "warning", flags, wxART_WARNING);
    switch(modalResult) {
        case wxYES: if (Save())
                        CanClose = clClose;
                    else
                        CanClose = clCancel;
            break;
        case wxYES_TO_ALL: if (Save())
                        CanClose = clCloseAllSave;
                    else
                        CanClose = clCancel;
                    break;
        case wxNO: CanClose = clClose; break;
        case wxNO_TO_ALL: CanClose = clCloseAllDiscard; break;
        default:
            CanClose = clCancel;
    }
}

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
ConsiderEnum Editor::Consider() {
    if (!IsModified() || IsEmpty())
        return coCanClose;
    else if (!path.empty())
        return coCanSave;
    else
        return coSaveAs;
}

void Editor::SetEditorStyle() {
    stc->StyleClearAll();

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
    stc->SetMarginWidth(0, 50);
    stc->SetMarginType(0, wxSTC_MARGIN_NUMBER);
    stc->SetMarginWidth(1, 16);
    stc->SetMarginType(1, wxSTC_MARGIN_SYMBOL);
    stc->SetMarginMask(1, wxSTC_MASK_FOLDERS);
    stc->StyleSetBackground(1, wxColor(200, 200, 200));
    stc->SetMarginSensitive(1, true);
    stc->SetMarginCursor(0,0);
    stc->SetMarginCursor(1,8);
    //line numbering
    stc->StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColour(128, 128, 128));
    stc->StyleSetBackground(wxSTC_STYLE_LINENUMBER, wxColour(242, 242, 242));

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
    stc->CmdKeyClearAll();
    stc->CmdKeyAssign('Z', wxSTC_KEYMOD_CTRL,wxSTC_CMD_UNDO);
    stc->CmdKeyAssign('Z', wxSTC_KEYMOD_SHIFT|wxSTC_KEYMOD_CTRL,wxSTC_CMD_REDO);
    stc->CmdKeyAssign('A', wxSTC_KEYMOD_CTRL,wxSTC_CMD_SELECTALL);
    stc->CmdKeyAssign('X', wxSTC_KEYMOD_CTRL,wxSTC_CMD_CUT);
    stc->CmdKeyAssign('C', wxSTC_KEYMOD_CTRL,wxSTC_CMD_COPY);
    stc->CmdKeyAssign('V', wxSTC_KEYMOD_CTRL,wxSTC_CMD_PASTE);
    stc->CmdKeyAssign(wxSTC_KEY_DELETE, wxSTC_KEYMOD_NORM, wxSTC_CMD_CLEAR);
    stc->CmdKeyAssign(wxSTC_KEY_DELETE, wxSTC_KEYMOD_SHIFT, wxSTC_CMD_CLEAR);
    stc->CmdKeyAssign(wxSTC_KEY_DOWN,  wxSTC_KEYMOD_NORM, wxSTC_CMD_LINEDOWN);
    stc->CmdKeyAssign(wxSTC_KEY_DOWN,  wxSTC_KEYMOD_SHIFT, wxSTC_CMD_LINEDOWNEXTEND);
    stc->CmdKeyAssign(wxSTC_KEY_UP,  wxSTC_KEYMOD_NORM, wxSTC_CMD_LINEUP);
    stc->CmdKeyAssign(wxSTC_KEY_UP,  wxSTC_KEYMOD_SHIFT, wxSTC_CMD_LINEUPEXTEND);
    stc->CmdKeyAssign(wxSTC_KEY_LEFT,  wxSTC_KEYMOD_NORM, wxSTC_CMD_CHARLEFT);
    stc->CmdKeyAssign(wxSTC_KEY_LEFT,  wxSTC_KEYMOD_SHIFT, wxSTC_CMD_CHARLEFTEXTEND);
    stc->CmdKeyAssign(wxSTC_KEY_RIGHT,  wxSTC_KEYMOD_NORM, wxSTC_CMD_CHARRIGHT);
    stc->CmdKeyAssign(wxSTC_KEY_RIGHT,  wxSTC_KEYMOD_SHIFT, wxSTC_CMD_CHARRIGHTEXTEND);
    stc->CmdKeyAssign(wxSTC_KEY_LEFT,  wxSTC_KEYMOD_CTRL, wxSTC_CMD_WORDLEFT);
    stc->CmdKeyAssign(wxSTC_KEY_LEFT,  wxSTC_KEYMOD_CTRL|wxSTC_KEYMOD_SHIFT, wxSTC_CMD_WORDLEFTEXTEND);
    stc->CmdKeyAssign(wxSTC_KEY_RIGHT,  wxSTC_KEYMOD_CTRL, wxSTC_CMD_WORDRIGHT);
    stc->CmdKeyAssign(wxSTC_KEY_RIGHT,  wxSTC_KEYMOD_CTRL|wxSTC_KEYMOD_SHIFT, wxSTC_CMD_WORDRIGHTENDEXTEND);
    //stc->CmdKeyAssign(wxSTC_KEY_HOME, wxSTC_KEYMOD_NORM, wxSTC_CMD_HOME);
    //stc->CmdKeyAssign(wxSTC_KEY_HOME, wxSTC_KEYMOD_SHIFT, wxSTC_CMD_HOMEEXTEND);
    stc->CmdKeyAssign(wxSTC_KEY_END, wxSTC_KEYMOD_NORM, wxSTC_CMD_LINEEND);
    stc->CmdKeyAssign(wxSTC_KEY_END, wxSTC_KEYMOD_SHIFT, wxSTC_CMD_LINEENDEXTEND);
    stc->CmdKeyAssign(wxSTC_KEY_HOME, wxSTC_KEYMOD_CTRL, wxSTC_CMD_DOCUMENTSTART);
    stc->CmdKeyAssign(wxSTC_KEY_HOME, wxSTC_KEYMOD_CTRL|wxSTC_KEYMOD_SHIFT, wxSTC_CMD_DOCUMENTSTARTEXTEND);
    stc->CmdKeyAssign(wxSTC_KEY_END, wxSTC_KEYMOD_CTRL, wxSTC_CMD_DOCUMENTEND);
    stc->CmdKeyAssign(wxSTC_KEY_END, wxSTC_KEYMOD_CTRL|wxSTC_KEYMOD_SHIFT, wxSTC_CMD_DOCUMENTENDEXTEND);
    stc->CmdKeyAssign(wxSTC_KEY_PRIOR, wxSTC_KEYMOD_NORM, wxSTC_CMD_PAGEUP);
    stc->CmdKeyAssign(wxSTC_KEY_PRIOR, wxSTC_KEYMOD_SHIFT, wxSTC_CMD_PAGEUPEXTEND);
    stc->CmdKeyAssign(wxSTC_KEY_NEXT, wxSTC_KEYMOD_NORM, wxSTC_CMD_PAGEDOWN);
    stc->CmdKeyAssign(wxSTC_KEY_NEXT, wxSTC_KEYMOD_SHIFT, wxSTC_CMD_PAGEDOWNEXTEND);
    stc->CmdKeyAssign(wxSTC_KEY_INSERT, wxSTC_KEYMOD_NORM, wxSTC_CMD_EDITTOGGLEOVERTYPE);
    stc->CmdKeyAssign(wxSTC_KEY_ESCAPE, wxSTC_KEYMOD_NORM, wxSTC_CMD_CANCEL);
    stc->CmdKeyAssign(wxSTC_KEY_BACK, wxSTC_KEYMOD_NORM, wxSTC_CMD_DELETEBACK);
    stc->CmdKeyAssign(wxSTC_KEY_BACK, wxSTC_KEYMOD_SHIFT, wxSTC_CMD_DELETEBACK);
    stc->CmdKeyAssign(wxSTC_KEY_TAB, wxSTC_KEYMOD_NORM, wxSTC_CMD_TAB);
    stc->CmdKeyAssign(wxSTC_KEY_TAB, wxSTC_KEYMOD_SHIFT, wxSTC_CMD_BACKTAB);
    stc->CmdKeyAssign(wxSTC_KEY_RETURN, wxSTC_KEYMOD_NORM, wxSTC_CMD_NEWLINE);
    stc->CmdKeyAssign(wxSTC_KEY_HOME, wxSTC_KEYMOD_NORM, wxSTC_CMD_VCHOME);
    stc->CmdKeyAssign(wxSTC_KEY_HOME, wxSTC_KEYMOD_SHIFT, wxSTC_CMD_VCHOMEEXTEND);
    stc->CmdKeyAssign('=', wxSTC_KEYMOD_CTRL, wxSTC_CMD_ZOOMIN);
    stc->CmdKeyAssign('-', wxSTC_KEYMOD_CTRL, wxSTC_CMD_ZOOMOUT);
    stc->CmdKeyAssign(wxSTC_KEY_BACK, wxSTC_KEYMOD_CTRL, wxSTC_CMD_DELWORDLEFT);
    stc->CmdKeyAssign(wxSTC_KEY_DELETE, wxSTC_KEYMOD_CTRL, wxSTC_CMD_DELWORDRIGHTEND);
    stc->CmdKeyAssign(wxSTC_KEY_DELETE, wxSTC_KEYMOD_CTRL|wxSTC_KEYMOD_SHIFT, wxSTC_CMD_DELWORDRIGHTEND);
    stc->CmdKeyAssign('Y', wxSTC_KEYMOD_CTRL,wxSTC_CMD_LINEDELETE);
    //stc->CmdKeyAssign('L', wxSTC_KEYMOD_CTRL,wxSTC_CMD_LOWERCASE);
    //stc->CmdKeyAssign('U', wxSTC_KEYMOD_CTRL,wxSTC_CMD_UPPERCASE);
    stc->CmdKeyAssign(wxSTC_KEY_UP,  wxSTC_KEYMOD_CTRL, wxSTC_CMD_LINESCROLLUP);
    stc->CmdKeyAssign(wxSTC_KEY_DOWN,  wxSTC_KEYMOD_CTRL, wxSTC_CMD_LINESCROLLDOWN);
    //stc->CmdKeyAssign(wxSTC_KEY_DELETE, wxSTC_KEYMOD_NORM, wxSTC_CMD_DELETEBACKNOTLINE);

}

void Editor::Reopen(bool AskWhenModified) {
    if (AskWhenModified && stc->IsModified())
        wxMessageBox("Reopen modified file");
    int pos = stc->GetCurrentPos();
    OpenFile(path);
    stc->SetCurrentPos(pos);
    stc->SetSelection(pos,pos);
    stc->EnsureCaretVisible();
}

void Editor::OpenFile(const wxString path) {
    this->path = path;
    if (!path.empty()) {
        wxFileName file(path);
        this->title = file.GetFullName();
        if (stc->LoadFile(path)) {

        } else {
            wxLogWarning(wxT("Can't load ") + path + wxT("!"));
        }
    }
    else this->title = "Untitled";
    SetEditorStyle();
    /*int maxPos=1000;
    stc->IndicatorClearRange(0, maxPos);
    stc->IndicatorSetStyle(1, wxSTC_INDIC_ROUNDBOX);
    stc->IndicatorSetAlpha(1, 100);
    stc->IndicatorSetUnder(1, true);
    stc->IndicatorSetForeground(1, wxColor(0, 255, 0));
    stc->SetIndicatorCurrent(1);
    stc->IndicatorFillRange(100, 20);*/
}

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

void Editor::StripTrailingSpaces() {
    const int maxLines = stc->GetLineCount();
    for (int line = 0; line < maxLines; line++) {
        const int lineStart = stc->GetLineSelStartPosition(line);
        const int lineEnd = stc->GetLineSelEndPosition(line);
        int i = lineEnd - 1;
        char ch = stc->GetCharAt(i);
        while ((i >= lineStart) && ((ch == ' ') || (ch == '\t'))) {
            i--;
            ch = stc->GetCharAt(i);
        }
        if (i < (lineEnd - 1)) {
            stc->SetTargetRange(i + 1, lineEnd);
            stc->ReplaceTarget("");
        }
    }
}

bool Editor::Save() {
    if (path.empty()) return false;
    StripTrailingSpaces();
    if (!stc->SaveFile(path)) return false;
    stc->SetModified(false);
    notebook->Refresh();
    return true;
}

void Editor::SaveAs(const wxString newpath) {
    path = newpath;
    wxFileName file(path);
    title = file.GetFullName();
    Save();
}

bool Editor::IsModified() {
    return stc->IsModified();
}

//Editor::GetHighLighter: TSynCustomHighlighter;() {}
//Editor::SetHighLighter(Value: TSynCustomHighlighter);() {}
//Editor::GetActivateTime: TDateTime;() {}
