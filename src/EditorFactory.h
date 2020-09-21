#ifndef SCINTAS_EDITORFACTORY_H
#define SCINTAS_EDITORFACTORY_H

#include <wx/aui/auibook.h>
#include "Editor.h"

class EditorFactory {
    wxAuiNotebook* notebook;
    std::vector<Editor*> list;
public:
    void TryCloseAll();
    Editor* CreateTabSheet(wxString path);
    int GetEditorCount();
    Editor* GetEditor(int Index);
    Editor* GetCurrentEditor();
    EditorFactory(wxAuiNotebook* auiNotebook);
    Editor *GetEditorByPath(const wxString &path);
    Editor *GetEditorByControl(const wxWindow* control);
    int GetEditorNumberByControl(const wxWindow *control);
    void CloseCurrent();
};


#endif //SCINTAS_EDITORFACTORY_H
