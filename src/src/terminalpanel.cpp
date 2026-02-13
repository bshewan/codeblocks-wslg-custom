#include "terminalpanel.h"
#include <wx/msgdlg.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/stream.h>
#include <wx/txtstrm.h>
#include <wx/utils.h>
#include <wx/aui/auibook.h>

BEGIN_EVENT_TABLE(TerminalPanel, wxPanel)
    EVT_END_PROCESS(wxID_ANY, TerminalPanel::OnEndProcess)
    EVT_TEXT_ENTER(wxID_ANY, TerminalPanel::OnTextEnter)
    EVT_TIMER(wxID_ANY, TerminalPanel::OnTimer)
    EVT_BUTTON(wxID_ANY, TerminalPanel::OnTestButton)
END_EVENT_TABLE()

TerminalPanel::TerminalPanel(wxWindow* parent)
    : wxPanel(parent, wxID_ANY),
      m_TextCtrl(nullptr),
      m_Process(nullptr),
      m_Timer(this, wxID_ANY),
      m_Pid(0),
      m_InputStart(0)
{
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    
    // Add test buttons
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* btnTest = new wxButton(this, wxID_ANY, "Test: echo Hello", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    wxButton* btnClear = new wxButton(this, wxID_ANY, "Clear", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    buttonSizer->Add(btnTest, 0, wxALL, 2);
    buttonSizer->Add(btnClear, 0, wxALL, 2);
    mainSizer->Add(buttonSizer, 0, wxEXPAND);
    
    m_TextCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString,
                                wxDefaultPosition, wxDefaultSize,
                                wxTE_MULTILINE | wxTE_RICH2 | wxTE_PROCESS_ENTER | wxTE_WORDWRAP);
    
    // Terminal-like appearance: larger monospace font, dark background
    m_TextCtrl->SetFont(wxFont(11, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    m_TextCtrl->SetBackgroundColour(wxColour(0, 0, 0));        // Black background
    m_TextCtrl->SetForegroundColour(wxColour(200, 200, 200));  // Light gray text
    
    // Connect char event for handling input
    m_TextCtrl->Bind(wxEVT_CHAR, &TerminalPanel::OnChar, this);
    
    mainSizer->Add(m_TextCtrl, 1, wxEXPAND | wxALL, 0);
    SetSizer(mainSizer);
    
    // Add welcome message
    m_TextCtrl->SetDefaultStyle(wxTextAttr(wxColour(100, 200, 255))); // Light blue
    m_TextCtrl->AppendText(_("Terminal Panel - Programs run here automatically\n\n"));
    m_TextCtrl->SetDefaultStyle(wxTextAttr(wxColour(200, 200, 200))); // Light gray
    m_InputStart = m_TextCtrl->GetLastPosition();
}

TerminalPanel::~TerminalPanel()
{
    StopProgram();
}

void TerminalPanel::Clear()
{
    m_TextCtrl->Clear();
    m_InputStart = 0;
    m_InputBuffer.Clear();
}

bool TerminalPanel::RunProgram(const wxString& program, const wxString& workingDir, const wxArrayString& env)
{
    if (IsRunning())
    {
        wxMessageBox(_("A program is already running in this terminal."), _("Error"), wxOK | wxICON_ERROR);
        return false;
    }
    
    // Activate the Terminal tab
    wxWindow* parent = GetParent();
    if (parent && parent->IsKindOf(CLASSINFO(wxAuiNotebook)))
    {
        wxAuiNotebook* notebook = static_cast<wxAuiNotebook*>(parent);
        int pageIndex = notebook->GetPageIndex(this);
        if (pageIndex != wxNOT_FOUND)
            notebook->SetSelection(pageIndex);
    }
    
    Clear();
    m_TextCtrl->SetDefaultStyle(wxTextAttr(wxColour(100, 200, 255))); // Light blue for info
    m_TextCtrl->AppendText(wxString::Format(_("Running: %s\n"), program));
    m_TextCtrl->SetDefaultStyle(wxTextAttr(wxColour(200, 200, 200))); // Light gray for output
    m_InputStart = m_TextCtrl->GetLastPosition();
    
    // Create process with redirected I/O
    m_Process = new wxProcess(this, wxID_ANY);
    m_Process->Redirect();
    
    // Set up environment
    wxExecuteEnv execEnv;
    if (!workingDir.IsEmpty())
        execEnv.cwd = workingDir;
    
    if (!env.IsEmpty())
    {
        wxGetEnvMap(&execEnv.env);
        for (size_t i = 0; i < env.GetCount(); ++i)
        {
            wxString envStr = env[i];
            size_t pos = envStr.Find('=');
            if (pos != wxNOT_FOUND)
            {
                wxString key = envStr.Mid(0, pos);
                wxString value = envStr.Mid(pos + 1);
                execEnv.env[key] = value;
            }
        }
    }
    
    // Launch the program
    int flags = wxEXEC_ASYNC;
    m_Pid = wxExecute(program, flags, m_Process, !env.IsEmpty() || !workingDir.IsEmpty() ? &execEnv : nullptr);
    
    if (m_Pid == 0)
    {
        m_TextCtrl->SetDefaultStyle(wxTextAttr(wxColour(255, 100, 100))); // Light red for errors
        m_TextCtrl->AppendText(_("Failed to start program.\n"));
        m_TextCtrl->SetDefaultStyle(wxTextAttr(wxColour(200, 200, 200))); // Back to light gray
        delete m_Process;
        m_Process = nullptr;
        return false;
    }
    
    // Start timer to poll for output (more frequently for better responsiveness)
    m_Timer.Start(25); // 25ms for faster output updates
    
    return true;
}

void TerminalPanel::StopProgram()
{
    if (m_Pid != 0)
    {
        m_Timer.Stop();
        wxProcess::Kill(m_Pid, wxSIGTERM);
        m_Pid = 0;
    }
    
    if (m_Process)
    {
        delete m_Process;
        m_Process = nullptr;
    }
}

void TerminalPanel::OnEndProcess(wxProcessEvent& event)
{
    m_Timer.Stop();
    
    // Read any remaining output
    ReadOutputStream();
    
    int exitCode = event.GetExitCode();
    m_TextCtrl->SetDefaultStyle(wxTextAttr(wxColour(100, 255, 100))); // Light green for success
    m_TextCtrl->AppendText(wxString::Format(_("\nProgram exited with code %d\n"), exitCode));
    m_TextCtrl->SetDefaultStyle(wxTextAttr(wxColour(200, 200, 200))); // Back to light gray
    
    delete m_Process;
    m_Process = nullptr;
    m_Pid = 0;
}

void TerminalPanel::OnTextEnter(wxCommandEvent& WXUNUSED(event))
{
    // This won't be called since we're handling chars manually
}

void TerminalPanel::OnChar(wxKeyEvent& event)
{
    if (!IsRunning())
    {
        event.Skip();
        return;
    }
    
    int keyCode = event.GetKeyCode();
    
    // Handle Enter key
    if (keyCode == WXK_RETURN || keyCode == WXK_NUMPAD_ENTER)
    {
        long currentPos = m_TextCtrl->GetInsertionPoint();
        if (currentPos >= m_InputStart)
        {
            // Get the input line
            wxString input = m_TextCtrl->GetRange(m_InputStart, m_TextCtrl->GetLastPosition());
            input += wxT("\n");
            
            // Send to program's stdin
            WriteToInput(input);
            
            // Echo the input in a different color to show it was sent
            m_TextCtrl->SetDefaultStyle(wxTextAttr(wxColour(150, 255, 150))); // Light green for user input
            m_TextCtrl->AppendText(wxT("\n"));
            m_TextCtrl->SetDefaultStyle(wxTextAttr(wxColour(200, 200, 200))); // Back to light gray
            m_InputStart = m_TextCtrl->GetLastPosition();
        }
        return;
    }
    
    // Handle Backspace - don't delete program output
    if (keyCode == WXK_BACK)
    {
        long currentPos = m_TextCtrl->GetInsertionPoint();
        if (currentPos <= m_InputStart)
        {
            // Don't allow backspace before input start
            return;
        }
    }
    
    // Only allow editing after m_InputStart
    long currentPos = m_TextCtrl->GetInsertionPoint();
    if (currentPos < m_InputStart && !event.ControlDown())
    {
        // Don't allow editing before the input start
        m_TextCtrl->SetInsertionPoint(m_TextCtrl->GetLastPosition());
        return;
    }
    
    // Set text color for user input as they type
    if (keyCode >= 32 && keyCode < 127) // Printable characters
    {
        m_TextCtrl->SetDefaultStyle(wxTextAttr(wxColour(255, 255, 150))); // Light yellow for typing
    }
    
    event.Skip();
}

void TerminalPanel::OnTimer(wxTimerEvent& WXUNUSED(event))
{
    if (IsRunning())
    {
        ReadOutputStream();
    }
}

void TerminalPanel::ReadOutputStream()
{
    if (!m_Process)
        return;
    
    bool hasOutput = false;
    
    // Read from stdout - read multiple chunks if available
    if (m_Process->IsInputAvailable())
    {
        wxInputStream* stream = m_Process->GetInputStream();
        while (stream && stream->CanRead())
        {
            char buffer[4096];
            stream->Read(buffer, sizeof(buffer) - 1);
            size_t bytesRead = stream->LastRead();
            if (bytesRead > 0)
            {
                buffer[bytesRead] = '\0';
                wxString output = wxString::FromUTF8(buffer);
                
                long savedPos = m_TextCtrl->GetInsertionPoint();
                m_TextCtrl->SetInsertionPoint(m_InputStart);
                m_TextCtrl->SetDefaultStyle(wxTextAttr(wxColour(200, 200, 200))); // Light gray for stdout
                m_TextCtrl->WriteText(output);
                m_InputStart = m_TextCtrl->GetInsertionPoint();
                m_TextCtrl->SetInsertionPoint(m_InputStart);
                hasOutput = true;
            }
            else
            {
                break; // No more data
            }
        }
    }
    
    // Read from stderr
    if (m_Process->IsErrorAvailable())
    {
        wxInputStream* stream = m_Process->GetErrorStream();
        while (stream && stream->CanRead())
        {
            char buffer[4096];
            stream->Read(buffer, sizeof(buffer) - 1);
            size_t bytesRead = stream->LastRead();
            if (bytesRead > 0)
            {
                buffer[bytesRead] = '\0';
                wxString output = wxString::FromUTF8(buffer);
                
                long savedPos = m_TextCtrl->GetInsertionPoint();
                m_TextCtrl->SetInsertionPoint(m_InputStart);
                m_TextCtrl->SetDefaultStyle(wxTextAttr(wxColour(255, 150, 150))); // Light red for stderr
                m_TextCtrl->WriteText(output);
                m_TextCtrl->SetDefaultStyle(wxTextAttr(wxColour(200, 200, 200))); // Back to light gray
                m_InputStart = m_TextCtrl->GetInsertionPoint();
                m_TextCtrl->SetInsertionPoint(m_InputStart);
                hasOutput = true;
            }
            else
            {
                break; // No more data
            }
        }
    }
    
    if (hasOutput)
    {
        m_TextCtrl->ShowPosition(m_TextCtrl->GetLastPosition());
    }
}

void TerminalPanel::WriteToInput(const wxString& text)
{
    if (!m_Process)
        return;
    
    wxOutputStream* stream = m_Process->GetOutputStream();
    if (stream)
    {
        wxCharBuffer buffer = text.ToUTF8();
        stream->Write(buffer.data(), buffer.length());
    }
}

void TerminalPanel::OnTestButton(wxCommandEvent& event)
{
    wxButton* btn = dynamic_cast<wxButton*>(event.GetEventObject());
    if (!btn)
        return;
    
    wxString label = btn->GetLabel();
    if (label.Contains("echo"))
    {
        RunProgram("/bin/echo 'Hello from Terminal Panel!'");
    }
    else if (label.Contains("Clear"))
    {
        Clear();
    }
}
