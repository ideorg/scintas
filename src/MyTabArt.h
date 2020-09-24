#ifndef SCINTAS_MYTABART_H
#define SCINTAS_MYTABART_H

#include "../3rdparty/auimod/tabart.h"

class MyTabArt: public wxAuiGenericTabArt {
public:
    wxColour GetFontColour(const wxAuiNotebookPage& page, const wxColour& back_color) wxOVERRIDE;
    wxAuiTabArt* Clone() wxOVERRIDE;
};


#endif //SCINTAS_MYTABART_H
