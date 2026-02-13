/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef EXAMINEMEMORYDLG_H
#define EXAMINEMEMORYDLG_H

#include <wx/panel.h>
#include <cbdebugger_interfaces.h>
#include <map>

class wxTextCtrl;
class CodeBlocksEvent;

class ExamineMemoryDlg : public wxPanel, public cbExamineMemoryDlg
{
    public:
        ExamineMemoryDlg(wxWindow* parent);
        ~ExamineMemoryDlg() override;

        wxWindow* GetWindow() override { return this; }

        // used for Freeze()/Thaw() calls
        void Begin() override;
        void End() override;

        void Clear() override;
        wxString GetBaseAddress() override;
        void SetBaseAddress(const wxString &addr) override;
        int GetBytes() override;
        void AddError(const wxString& err) override;
        void AddHexByte(const wxString& addr, const wxString& hexbyte) override;
        void EnableWindow(bool enable) override;
    protected:
        void OnGo(wxCommandEvent& event);
        void OnDebuggerContinued(CodeBlocksEvent& event);

    private:
        wxTextCtrl* m_pText;
        size_t m_ByteCounter;
        wxChar m_LineText[67]; // 16*3 "7F " + 3 "   " + 16 "."
        uint64_t m_LastRowStartingAddress;
        
        // Storage for previous memory values to detect changes
        std::map<uint64_t, unsigned char> m_PreviousValues;
        bool m_HighlightChanges;
        bool m_ChangedBytes[16];  // Track which bytes in current line have changed
        int m_HighlightWordSize;  // Size in bytes for highlighting (1, 2, 4, or 8)
        
        void ResetHighlighting();
    private:
        DECLARE_EVENT_TABLE()
};

#endif // EXAMINEMEMORYDLG_H
