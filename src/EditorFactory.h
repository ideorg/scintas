#ifndef SCINTAS_EDITORFACTORY_H
#define SCINTAS_EDITORFACTORY_H

#include "../3rdparty/auimod/auibook.h"
#include "Editor.h"

class EditorFactory {
    wxAuiNotebook* notebook;
public:
    void TryCloseAll();
    Editor* CreateTabSheet(wxString path);
    int GetEditorCount();
    Editor* GetEditor(int Index);
    Editor* GetCurrentEditor();
    EditorFactory(wxAuiNotebook* auiNotebook);
    Editor *GetEditorByPath(const wxString &path);
    void CloseCurrent();
    void CloseEditorForPage(int page);
};


#endif //SCINTAS_EDITORFACTORY_H
