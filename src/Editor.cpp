#include "Editor.h"
#include <wx/filename.h>

Editor::Editor(wxStyledTextCtrl* stc, wxWindowID id):stc(stc),id(id) {}

void Editor::Activate() { }
void Editor::ActionsBeforeClose() {}
void Editor::AskSaveChangesBeforeClosing(CloseEnum &CanClose) {}
CloseEnum Editor::AskSaveChangesBeforeReopen() { return clClose;}
bool Editor::CanClose() { return true; }
void Editor::TryClose() {  }
wxStyledTextCtrl* Editor::GetWidget() { return stc; };
//function Editor::GetCaretPos: TPoint;
//function Editor::GetStateStr: string;
//std::string Editor::GetFileName() { return ""; }
wxString Editor::GetTitle() {
    return title;
}
//Editor::GetLines: TStrings;() override;
//Editor::GetCurrentLine: string;() override;
//Editor::LogicalToPhysicalCol(const Line: String; Index, LogicalPos: integer): integer;() override;
//bool Editor::GetModified() {  }
ConsiderEnum Editor::Consider() { return coCanClose;}
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
}
void Editor::Reopen(bool AskWhenModified) {}
FileChangeKind Editor::ChangedOutside() {return fckNothing;}
void Editor::SpecialChars(bool Show) {}
//Editor::GetHighLighter: TSynCustomHighlighter;() {}
//Editor::SetHighLighter(Value: TSynCustomHighlighter);() {}
//Editor::GetActivateTime: TDateTime;() {}
