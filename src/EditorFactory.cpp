#include "EditorFactory.h"

EditorFactory::EditorFactory(wxAuiNotebook* auiNotebook): notebook(auiNotebook) {}
void EditorFactory::TryCloseAll() { }

static long GetNewEditorId()
{
    static int id = 10000;
    return id++;
}

Editor* EditorFactory::CreateTabSheet(wxString path) {
    wxWindowID new_id = GetNewEditorId();
    wxStyledTextCtrl* stc = new wxStyledTextCtrl(notebook, new_id, wxDefaultPosition, wxDefaultSize, 0L, "");
    Editor *editor = new Editor(stc, new_id, notebook);
    editor->OpenFile(path);
    notebook->AddPageEx(stc, editor, editor->GetTitle(), true);
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

Editor *EditorFactory::GetEditorByPath(const wxString &path) {
    int count = GetEditorCount();
    for (int i=0; i<count; i++) {
        Editor* editor = GetEditor(i);
        if (editor->GetPath()==path)
            return editor;
    }

    return nullptr;
}

bool EditorFactory::CloseEditor(int n) {
    Editor *editor = GetEditor(n);
    ConsiderEnum consider = editor->Consider();
    if (consider!=coCanClose) {
        CloseEnum canClose = clClose;
        editor->AskSaveChangesBeforeClosing(canClose);
        if (canClose==clCancel) return false;
    }
    delete editor;
    return true;
}

void EditorFactory::CloseCurrent() {
    if (GetEditorCount()==0) return;
    int n = notebook->GetSelection();
    if (CloseEditor(n))
        notebook->DeletePage(n);
}
