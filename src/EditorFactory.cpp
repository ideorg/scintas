#include "EditorFactory.h"

EditorFactory::EditorFactory(wxAuiNotebook* Owner):notebook(Owner) {}
void EditorFactory::TryCloseAll() { }

static long GetNewEditorId()
{
    static int id = 10000;
    return id++;
}

Editor* EditorFactory::CreateTabSheet(wxString path) {
    wxWindowID new_id = GetNewEditorId();
    wxStyledTextCtrl* stc = new wxStyledTextCtrl(notebook, new_id, wxDefaultPosition, wxDefaultSize, 0L, "");
    Editor *editor = new Editor(stc, new_id);
    editor->OpenFile(path);
    notebook->AddPage(stc, editor->GetTitle(), true);
    list.push_back(editor);
    return editor;
}

int EditorFactory::GetEditorCount() { return list.size(); }
Editor* EditorFactory::GetEditor(int Index) { return list[Index]; }