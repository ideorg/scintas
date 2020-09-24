#ifndef SCINTAS_EDITOR_H
#define SCINTAS_EDITOR_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <string>
#include <wx/stc/stc.h>
#include "../3rdparty/auimod/auibook.h"

enum CloseEnum {clClose, clCloseAllSave, clCloseAllDiscard, clNo, clCancel, clError};
enum ConsiderEnum {coCanClose, coCanSave, coSaveAs};
enum FileChangeKind {fckNothing, fckRemoved, fckEmpty, fckChanged, fckLooksIdentical};

class Editor: public wxObject {
    wxStyledTextCtrl* stc;
    wxWindowID id;
    wxString path;
    wxString title;
    void SetEditorStyle();
    wxAuiNotebook* notebook;
public:
    virtual void Activate();
    virtual void ActionsBeforeClose();
    virtual void AskSaveChangesBeforeClosing(CloseEnum &CanClose);
    virtual CloseEnum AskSaveChangesBeforeReopen();
    virtual bool CanClose();
    virtual void TryClose();
    virtual wxStyledTextCtrl* GetWidget();
    //function GetCaretPos: TPoint;
    //function GetStateStr: string;
    virtual wxString GetPath();
    virtual wxString GetTitle();
    //virtual function GetLines: TStrings;();
    //virtual function GetCurrentLine: string;();
    //virtual function LogicalToPhysicalCol(const Line: String; Index, LogicalPos: integer): integer;();
    //virtual bool GetModified();
    virtual ConsiderEnum Consider();
    virtual void OpenFile(const wxString path);
    virtual void Reopen(bool AskWhenModified);
    virtual FileChangeKind ChangedOutside();
    virtual void SpecialChars(bool Show);
    void Save();
    void SaveAs(const wxString newpath);
    void InsertDate();
    void InsertTime();
    void InsertDateTime();
    //virtual function GetHighLighter: TSynCustomHighlighter;();
    //virtual procedure SetHighLighter(Value: TSynCustomHighlighter);();
    //virtual function GetActivateTime: TDateTime;();
    Editor(wxStyledTextCtrl* stc, wxWindowID id, wxAuiNotebook* notebook);
    wxWindowID GetId() {return id;}
};


#endif //SCINTAS_EDITOR_H
