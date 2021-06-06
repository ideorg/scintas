#ifndef SCINTAS_COMPAT_H
#define SCINTAS_COMPAT_H

#ifdef wxOVERRIDE
#define new_wxWidgets
#else
#undef new_wxWidgets
    #define wxOVERRIDE override
#endif
#include "wx/stc/stc.h"

#define wxSTC_KEYMOD_NORM wxSTC_SCMOD_NORM
#define wxSTC_KEYMOD_SHIFT wxSTC_SCMOD_SHIFT
#define wxSTC_KEYMOD_CTRL wxSTC_SCMOD_CTRL
#define wxSTC_KEYMOD_ALT wxSTC_SCMOD_ALT
#define wxSTC_KEYMOD_SUPER wxSTC_SCMOD_SUPER
#define wxSTC_KEYMOD_META wxSTC_SCMOD_META

#define SCI_SETTARGETRANGE 2686

void STC_SetTargetRange(wxStyledTextCtrl *stc, int start, int end)
{
    stc->SendMsg(SCI_SETTARGETRANGE, start, end);
}

#endif //SCINTAS_COMPAT_H
