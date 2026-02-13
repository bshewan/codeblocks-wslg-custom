#include "terminal_accessor.h"

namespace TerminalAccess {
    static ITerminalPanel* g_TerminalPanel = nullptr;
    
    void SetTerminalPanel(ITerminalPanel* panel) {
        g_TerminalPanel = panel;
    }
    
    ITerminalPanel* GetTerminalPanel() {
        return g_TerminalPanel;
    }
}
