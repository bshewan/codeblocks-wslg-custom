/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision: 12285 $
 * $Id: examinememorydlg.cpp 12285 2021-01-02 14:06:28Z fuscated $
 * $HeadURL: https://svn.code.sf.net/p/codeblocks/code/branches/release-25.03/src/src/examinememorydlg.cpp $
 */

#include "sdk.h"

#ifndef CB_PRECOMP
    #include <wx/button.h>
    #include <wx/combobox.h>
    #include <wx/intl.h>
    #include <wx/textctrl.h>
    #include <wx/xrc/xmlres.h>

    #include "cbplugin.h"
#endif

#include "examinememorydlg.h"
#include "debuggermanager.h"
#include "sdk_events.h"

#include <cinttypes> // For PRIx64

BEGIN_EVENT_TABLE(ExamineMemoryDlg, wxPanel)
    EVT_BUTTON(XRCID("btnGo"), ExamineMemoryDlg::OnGo)
    EVT_COMBOBOX(XRCID("cmbBytes"), ExamineMemoryDlg::OnGo)
    EVT_TEXT_ENTER(XRCID("txtAddress"), ExamineMemoryDlg::OnGo)
END_EVENT_TABLE()

ExamineMemoryDlg::ExamineMemoryDlg(wxWindow* parent) :
    m_LastRowStartingAddress(0),
    m_HighlightChanges(true),
    m_FreezeAddress(false)
{
    //ctor
    if (!wxXmlResource::Get()->LoadPanel(this, parent, _T("MemoryDumpPanel")))
        return;
    m_pText = XRCCTRL(*this, "txtDump", wxTextCtrl);

    wxFont font(12, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    m_pText->SetFont(font);

    ConfigManager *c = Manager::Get()->GetConfigManager(wxT("debugger_common"));
    int bytes = c->ReadInt(wxT("/common/examine_memory/size_to_show"), 32);
    wxString strBytes;
    strBytes << bytes;
    wxComboBox *combo = XRCCTRL(*this, "cmbBytes", wxComboBox);
    if (!combo->SetStringSelection(strBytes))
        combo->SetSelection(1); // Default is 32 bytes

    // Set highlight word size based on configuration
    // Default to 4 bytes which works well for most common types:
    // - 32-bit integers (int, uint32_t)
    // - 32-bit floats
    // - Most variables in typical code
    // Users can configure to 8 bytes for pointer-heavy debugging
    m_HighlightWordSize = c->ReadInt(wxT("/common/examine_memory/highlight_word_size"), 4);
    
    // Validate word size (must be 1, 2, 4, or 8)
    if (m_HighlightWordSize != 1 && m_HighlightWordSize != 2 && 
        m_HighlightWordSize != 4 && m_HighlightWordSize != 8)
    {
        m_HighlightWordSize = 4;  // Fall back to 4 bytes
    }

    for (int i = 0; i < 16; ++i)
        m_ChangedBytes[i] = false;

    Clear();
    
    // Register for debugger events
    typedef cbEventFunctor<ExamineMemoryDlg, CodeBlocksEvent> EventFunctor;
    Manager::Get()->RegisterEventSink(cbEVT_DEBUGGER_CONTINUED,
        new EventFunctor(this, &ExamineMemoryDlg::OnDebuggerContinued));
}

ExamineMemoryDlg::~ExamineMemoryDlg()
{
    Manager::Get()->RemoveAllEventSinksFor(this);
}

void ExamineMemoryDlg::Begin()
{
    m_pText->Freeze();
    m_HighlightChanges = true;  // Re-enable highlighting when new data arrives
    for (int i = 0; i < 16; ++i)
        m_ChangedBytes[i] = false;
}

void ExamineMemoryDlg::End()
{
    m_pText->Thaw();
}

void ExamineMemoryDlg::Clear()
{
    m_pText->Clear();
    m_LastRowStartingAddress = 0;
    m_ByteCounter = 0;
    for (int i = 0; i < 67; ++i)
        m_LineText[i] = _T(' ');
}

wxString ExamineMemoryDlg::GetBaseAddress()
{
    return XRCCTRL(*this, "txtAddress", wxTextCtrl)->GetValue();
}

int ExamineMemoryDlg::GetBytes()
{
    long a;
    XRCCTRL(*this, "cmbBytes", wxComboBox)->GetValue().ToLong(&a);
    return a;
}

void ExamineMemoryDlg::AddError(const wxString& err)
{
    m_pText->AppendText(err + _T('\n'));
}

void ExamineMemoryDlg::AddHexByte(const wxString& addr, const wxString& hexbyte)
{
//    m_pDbg->Log(_T("AddHexByte(") + addr + _T(", ") + hexbyte + _T(')'));
    int bcmod = m_ByteCounter % 16;

    // Reset changed bytes tracking at start of each new line
    if (m_ByteCounter % 16 == 0)
    {
        for (int i = 0; i < 16; ++i)
            m_ChangedBytes[i] = false;
    }

    if (m_ByteCounter == 0)
    {
        // because we 'll be appending each row *after* we have consumed it
        // and then "addr" will point to the next row's starting address,
        // we 'll keep the current row's starting address in "m_LastRowStartingAddress".

        // if it's zero (i.e this is the first row), keep "addr" as starting address for this row.
        // m_LastRowStartingAddress will be set again when we 've consumed this row...
        m_LastRowStartingAddress = cbDebuggerStringToAddress(addr);
        
        // If freeze address flag is set, replace the address textbox with the resolved hex address
        // This "freezes" expressions like #$rsp to their evaluated value
        if (m_FreezeAddress)
        {
            wxTextCtrl *addrCtrl = XRCCTRL(*this, "txtAddress", wxTextCtrl);
            wxString hexAddr = wxString::Format(wxT("0x%llx"), (unsigned long long)m_LastRowStartingAddress);
            addrCtrl->SetValue(hexAddr);
            m_FreezeAddress = false;  // Only freeze once
        }
    }

#define HEX_OFFSET(a) (a*3)
#define CHAR_OFFSET(a) (16*3 + 3 + a)

    unsigned long hb;
    hexbyte.ToULong(&hb, 16);
    
    // Check if this byte has changed from previous value
    uint64_t byteAddress = m_LastRowStartingAddress + bcmod;
    bool hasChanged = false;
    if (m_PreviousValues.find(byteAddress) != m_PreviousValues.end())
    {
        hasChanged = (m_PreviousValues[byteAddress] != (unsigned char)hb);
    }
    
    // Store current value for next comparison
    m_PreviousValues[byteAddress] = (unsigned char)hb;
    
    // Mark this byte as changed if it differs from previous
    // AND mark the entire word (of configured size) containing this byte
    if (hasChanged && m_HighlightChanges)
    {
        // Calculate the word boundary based on m_HighlightWordSize
        // For word size N: bytes are grouped as 0-(N-1), N-(2N-1), 2N-(3N-1), etc.
        int wordStart = (bcmod / m_HighlightWordSize) * m_HighlightWordSize;
        int wordEnd = wordStart + m_HighlightWordSize;
        
        // Mark all bytes in this word as changed
        for (int i = wordStart; i < wordEnd && i < 16; ++i)
        {
            m_ChangedBytes[i] = true;
        }
    }
    
//    m_pDbg->Log(wxString::Format(_T("hb=%d, [0]=%c, [1]=%c"), hb, hexbyte[0], hexbyte[1]));
//    m_pDbg->Log(wxString::Format(_T("HEX_OFFSET(bcmod)=%d, CHAR_OFFSET(bcmod)=%d"), HEX_OFFSET(bcmod), CHAR_OFFSET(bcmod)));
    m_LineText[HEX_OFFSET(bcmod)] = hexbyte[0];
    m_LineText[HEX_OFFSET(bcmod) + 1] = hexbyte[1];
    m_LineText[CHAR_OFFSET(bcmod)] = hb >= 32 ? wxChar(hb) : wxChar(_T('.'));
    
    ++m_ByteCounter;

    // flush every 16 bytes
    if (m_ByteCounter != 0 && m_ByteCounter % 16 == 0)
    {
        // filled 16 bytes window; append text and reset accumulator array
        if (m_ByteCounter != 16) // after the first line,
            m_pText->AppendText(_T('\n')); // prepend a newline
        m_LineText[23] = _T('|'); // put a "separator" in the middle (just to ease reading a bit)

        // Build the line with address prefix
        wxString addressPart = wxString::Format(_T("0x%" PRIx64 ": "), m_LastRowStartingAddress);
        m_pText->AppendText(addressPart);
        
        // Now append the hex bytes and ASCII representation with appropriate colors
        for (int i = 0; i < 67; ++i)
        {
            // Determine if this position is part of a changed byte
            bool isChanged = false;
            int byteIndex = -1;
            
            // Check if position is in hex area (bytes 0-47, positions 0-2, 3-5, etc.)
            if (i < 48)
            {
                byteIndex = i / 3;  // Each byte takes 3 chars (2 hex + space)
                if (i % 3 != 2)  // Not the space character
                    isChanged = (byteIndex < 16) && m_ChangedBytes[byteIndex];
            }
            // Check if position is in ASCII area (after position 51)
            else if (i >= 51)
            {
                byteIndex = i - 51;
                if (byteIndex < 16)
                    isChanged = m_ChangedBytes[byteIndex];
            }
            
            if (isChanged && m_HighlightChanges)
            {
                // Set red color for changed bytes
                wxTextAttr redAttr(*wxRED);
                m_pText->SetDefaultStyle(redAttr);
            }
            else
            {
                // Reset to default color
                wxTextAttr defaultAttr(m_pText->GetForegroundColour());
                m_pText->SetDefaultStyle(defaultAttr);
            }
            
            m_pText->AppendText(wxString(m_LineText[i]));
        }
        
        // Reset to default style
        wxTextAttr defaultAttr(m_pText->GetForegroundColour());
        m_pText->SetDefaultStyle(defaultAttr);
        
        for (int i = 0; i < 67; ++i)
            m_LineText[i] = _T(' ');
        // update starting address for next row every 16 bytes
        m_LastRowStartingAddress += 16;
    }
}

void ExamineMemoryDlg::OnGo(cb_unused wxCommandEvent& event)
{
    cbDebuggerPlugin *plugin = Manager::Get()->GetDebuggerManager()->GetActiveDebugger();

    // Save the value of the bytes combo box in the config,
    // so it is the same next time the dialog is used.
    ConfigManager *c = Manager::Get()->GetConfigManager(wxT("debugger_common"));
    c->Write(wxT("/common/examine_memory/size_to_show"), GetBytes());

    // Check if address starts with '#' - this means "freeze" the expression
    // by evaluating it once and replacing with the actual hex address
    wxTextCtrl *addrCtrl = XRCCTRL(*this, "txtAddress", wxTextCtrl);
    wxString address = addrCtrl->GetValue().Trim().Trim(false);
    
    if (address.StartsWith(wxT("#")))
    {
        // Strip the '#' prefix for evaluation
        address = address.Mid(1).Trim(false);
        
        // Temporarily set the address without '#' so debugger can evaluate it
        addrCtrl->SetValue(address);
        
        // Mark that we need to freeze this address after evaluation
        // We'll do this by storing the flag and handling it in AddHexByte
        // where we can see the resolved address
        m_FreezeAddress = true;
    }
    else
    {
        m_FreezeAddress = false;
    }

    if (plugin)
        plugin->RequestUpdate(cbDebuggerPlugin::ExamineMemory);
}

void ExamineMemoryDlg::EnableWindow(bool enable)
{
    Enable(enable);
}

void ExamineMemoryDlg::SetBaseAddress(const wxString &addr)
{
    XRCCTRL(*this, "txtAddress", wxTextCtrl)->SetValue(addr);

    cbDebuggerPlugin *plugin = Manager::Get()->GetDebuggerManager()->GetActiveDebugger();
    if (plugin)
        plugin->RequestUpdate(cbDebuggerPlugin::ExamineMemory);

}

void ExamineMemoryDlg::ResetHighlighting()
{
    // Don't clear previous values - we need them for next comparison
    // Just disable highlighting temporarily or clear the stored values
    // For now, we'll keep the values and let highlighting continue
    // This gets called when debugger continues
    m_HighlightChanges = false;
}

void ExamineMemoryDlg::OnDebuggerContinued(CodeBlocksEvent& event)
{
    // When debugger continues, reset text colors on next update
    // We keep the previous values for comparison but stop highlighting
    // The highlighting will resume when we pause again and new values come in
    ResetHighlighting();
    event.Skip();
}
