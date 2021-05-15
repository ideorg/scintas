#include "EditorFactory.h"
#include <wx/clipbrd.h>

EditorFactory::EditorFactory(wxAuiNotebook* auiNotebook): notebook(auiNotebook) {}
void EditorFactory::TryCloseAll() { }

static long GetNewEditorId()
{
    static int id = 10000;
    return id++;
}

static long GetNewUntitledId()
{
    static int id = 0;
    return ++id;
}

Editor* EditorFactory::CreateTabSheet(wxString path) {
    wxWindowID new_id = GetNewEditorId();
    wxWindowID untitled_id = wxIsEmpty(path)?GetNewUntitledId():0;
    wxStyledTextCtrl* stc = new wxStyledTextCtrl(notebook, new_id, wxDefaultPosition, wxDefaultSize, 0L, "");
    Editor *editor = new Editor(stc, new_id, untitled_id, notebook);
    editor->OpenFile(path);
    notebook->AddPageEx(stc, editor, editor->GetTitle(), true);
    stc->SetFocus();
    return editor;
}

int EditorFactory::GetEditorCount() {
    return notebook->GetPageCount();
}

Editor* EditorFactory::GetEditor(int Index) {
    return dynamic_cast<Editor *>(notebook->GetPageEx(Index)->object);
}

Editor* EditorFactory::GetCurrentEditor() {
    if (GetEditorCount()==0) return nullptr;
    return GetEditor(notebook->GetSelection());
}

wxStyledTextCtrl * EditorFactory::GetCurrentWidget() {
    if (GetEditorCount()==0) return nullptr;
    int n = notebook->GetSelection();
    return dynamic_cast<wxStyledTextCtrl *>(notebook->GetPage(n));
}

Editor *EditorFactory::GetEditorByPath(const wxString &path) {
    int count = GetEditorCount();
    for (int i=0; i<count; i++) {
        Editor* editor = GetEditor(i);
        if (editor->GetPath()==path)
            return editor;
    }

    return nullptr;
}

void EditorFactory::CloseEditor(int n, bool extended, CloseEnum &closeEnum) {
    Editor *editor = GetEditor(n);
    ConsiderEnum consider = editor->Consider();
    if (consider!=coCanClose) {
        editor->AskSaveChangesBeforeClosing(closeEnum, extended);
        if (closeEnum == clCancel) {
            closeEnum = clCancel;
            return;
        }
    }
}

void EditorFactory::CloseCurrent() {
    if (GetEditorCount()==0) return;
    int n = notebook->GetSelection();
    CloseEnum closeEnum = clClose;
    CloseEditor(n, false, closeEnum);
    if (closeEnum!=clCancel) {
        wxTheClipboard->Flush();
        notebook->DeletePage(n);
    }
}
