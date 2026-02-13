#ifndef TERMINAL_ACCESSOR_H
#define TERMINAL_ACCESSOR_H

#include <wx/string.h>
#include <wx/arrstr.h>

// Forward declare to avoid circular dependency
class wxWindow;

// Interface for terminal panel functionality
class ITerminalPanel {
public:
    virtual ~ITerminalPanel() {}
    virtual bool RunProgram(const wxString& program, const wxString& workingDir = wxEmptyString, const wxArrayString& env = wxArrayString()) = 0;
    virtual void Clear() = 0;
    virtual bool IsRunning() const = 0;
    virtual wxWindow* GetWindow() = 0;
};

// Global accessor for terminal panel
namespace TerminalAccess {
    void SetTerminalPanel(ITerminalPanel* panel);
    ITerminalPanel* GetTerminalPanel();
}

#endif // TERMINAL_ACCESSOR_H
