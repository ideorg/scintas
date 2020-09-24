#include "MyTabArt.h"
#include "Editor.h"

wxColour MyTabArt::GetFontColour(const wxAuiNotebookPage& page, const wxColour& back_color) {
    if (page.object && dynamic_cast<Editor *>(page.object)->IsModified())
        return wxColour(255,50,50);
    else
        return wxAuiGenericTabArt::GetFontColour(page,back_color);
}

wxAuiTabArt *MyTabArt::Clone() {
    return new MyTabArt(*this);
}
