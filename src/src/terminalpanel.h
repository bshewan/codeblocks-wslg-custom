#ifndef TERMINALPANEL_H
#define TERMINALPANEL_H

#include <wx/panel.h>
#include <wx/textctrl.h>
#include <wx/process.h>
#include <wx/timer.h>
#include "terminal_accessor.h"

class TerminalPanel : public wxPanel, public ITerminalPanel
{
public:
    TerminalPanel(wxWindow* parent);
    ~TerminalPanel();
    
    // ITerminalPanel implementation
    bool RunProgram(const wxString& program, const wxString& workingDir = wxEmptyString, const wxArrayString& env = wxArrayString()) override;
    void Clear() override;
    bool IsRunning() const override { return m_Pid != 0; }
    wxWindow* GetWindow() override { return this; }
    
    // Stop the currently running program
    void StopProgram();

private:
    void OnEndProcess(wxProcessEvent& event);
    void OnTextEnter(wxCommandEvent& event);
    void OnTimer(wxTimerEvent& event);
    void OnChar(wxKeyEvent& event);
    void OnTestButton(wxCommandEvent& event);
    
    void ReadOutputStream();
    void WriteToInput(const wxString& text);
    
    wxTextCtrl* m_TextCtrl;
    wxProcess* m_Process;
    wxTimer m_Timer;
    long m_Pid;
    long m_InputStart;
    wxString m_InputBuffer;
    
    DECLARE_EVENT_TABLE()
};

#endif // TERMINALPANEL_H
