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
    notebook->AddPage(stc, editor->GetTitle(), true);
    list.push_back(editor);
    return editor;
}

int EditorFactory::GetEditorCount() { return list.size(); }
Editor* EditorFactory::GetEditor(int Index) { return list[Index]; }
Editor* EditorFactory::GetCurrentEditor() {
    if (list.empty()) return nullptr;
    int n = notebook->GetSelection();
    wxWindow* stc = notebook->GetPage(n);
    return GetEditorByControl(stc);
}

Editor *EditorFactory::GetEditorByPath(const wxString &path) {
    for (auto editor: list)
        if (editor->GetPath()==path)
            return editor;
    return nullptr;
}

Editor *EditorFactory::GetEditorByControl(const wxWindow *control) {
    for (auto editor: list)
        if (editor->GetWidget()==control)
            return editor;
    return nullptr;
}
