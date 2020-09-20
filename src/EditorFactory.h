#ifndef SCINTAS_EDITORFACTORY_H
#define SCINTAS_EDITORFACTORY_H

#include <wx/aui/auibook.h>
#include "Editor.h"

class EditorFactory {
public:
    void TryCloseAll();
    Editor* CreateTabSheet(wxString path);
    int GetEditorCount();
    Editor* GetEditor(int Index);
    //void Swap(int FirstIdx, int SecondIdx) override;
private:
    wxAuiNotebook* notebook;
    std::vector<Editor*> list;
public:
    EditorFactory(wxAuiNotebook* auiNotebook);
    Editor *GetEditorByPath(const wxString &path);
};


#endif //SCINTAS_EDITORFACTORY_H
