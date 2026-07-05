#include <wx/wx.h>
#include <wx/dcbuffer.h>
#include <vector>
#include <string>

struct MorseNode {
    char letter;
    wxPoint position;
    bool isCircle;
    std::string path;
};

class MorseWindow : public wxFrame {
public:
    MorseWindow() : wxFrame(nullptr, wxID_ANY, "Morse Code Visualizer", wxDefaultPosition, wxSize(500, 740), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)) {
        SetBackgroundStyle(wxBG_STYLE_PAINT);

        wxIcon icon;
        icon.CopyFromBitmap(wxBitmap("icon.png", wxBITMAP_TYPE_PNG));
        SetIcon(icon);

        nodes = {
            {'E', wxPoint(280, 80),  true,  "."},
            {'I', wxPoint(340, 80),  true,  ".."},
            {'S', wxPoint(400, 80),  true,  "..."},
            {'H', wxPoint(460, 80),  true,  "...."},
            {'U', wxPoint(340, 160), false, "..-"},
            {'V', wxPoint(400, 160), false, "...-"},
            {'F', wxPoint(340, 240), true,  "..-."},
            {'A', wxPoint(280, 310), false, ".-"},
            {'R', wxPoint(340, 310), true,  ".-."},
            {'L', wxPoint(400, 310), true,  ".-.."},

            {'W', wxPoint(280, 500), false, ".--"},
            {'P', wxPoint(340, 500), true,  ".--."},
            {'J', wxPoint(280, 590), false, ".---"},

            {'T', wxPoint(160, 80),  false, "-"},
            {'M', wxPoint(100, 80),  false, "--"},
            {'O', wxPoint(40, 80),   false, "---"},
            {'G', wxPoint(100, 160), true,  "--."},
            {'Q', wxPoint(40, 160),  false, "--.-"},
            {'Z', wxPoint(100, 240), true,  "--.."},
            {'N', wxPoint(160, 310), true,  "-."},
            {'K', wxPoint(100, 310), false, "-.-"},
            {'Y', wxPoint(40, 310),  false, "-.--"},
            {'C', wxPoint(100, 410), true,  "-.-."},

            {'X', wxPoint(100, 500), false, "-..-"},
            {'D', wxPoint(160, 500), true,  "-.."},
            {'B', wxPoint(160, 590), true,  "-..."}
        };

        Bind(wxEVT_PAINT, &MorseWindow::OnPaint, this);
        Bind(wxEVT_KEY_DOWN, &MorseWindow::OnKeyDown, this);

        releaseTimer.SetOwner(this, ID_RELEASE_TIMER);
        letterTimer.SetOwner(this, ID_LETTER_TIMER);
        wordGapTimer.SetOwner(this, ID_WORD_TIMER);
        Bind(wxEVT_TIMER, &MorseWindow::OnReleaseTimer, this, ID_RELEASE_TIMER);
        Bind(wxEVT_TIMER, &MorseWindow::OnLetterTimer, this, ID_LETTER_TIMER);
        Bind(wxEVT_TIMER, &MorseWindow::OnWordGapTimer, this, ID_WORD_TIMER);

        SetFocus();
    }

private:
    enum {
        ID_RELEASE_TIMER = wxID_HIGHEST + 1,
        ID_LETTER_TIMER,
        ID_WORD_TIMER
    };

    static const int SignalTimeoutMs = 300;
    static const int LetterGapMs = 450;
    static const int WordGapMs = 1200;

    std::vector<MorseNode> nodes;
    std::string currentMorse = "";
    std::string typedText = "";
    wxTimer releaseTimer;
    wxTimer letterTimer;
    wxTimer wordGapTimer;
    bool dashBurstActive = false;

    void OnPaint(wxPaintEvent& event) {
        wxAutoBufferedPaintDC dc(this);
        dc.SetBackground(wxBrush(wxColor(20, 24, 33)));
        dc.Clear();

        wxPen linePen(wxColor(90, 90, 90), 2);
        dc.SetPen(linePen);

        dc.DrawLine(220, 80, 460, 80);
        dc.DrawLine(220, 80, 40, 80);

        dc.DrawLine(340, 80, 340, 240);
        dc.DrawLine(400, 80, 400, 160);

        dc.DrawLine(280, 80, 280, 590);
        dc.DrawLine(280, 310, 400, 310);
        dc.DrawLine(280, 500, 340, 500);

        dc.DrawLine(100, 80, 100, 240);
        dc.DrawLine(100, 160, 40, 160);

        dc.DrawLine(160, 80, 160, 590);
        dc.DrawLine(160, 310, 40, 310);
        dc.DrawLine(100, 310, 100, 410);
        dc.DrawLine(160, 500, 100, 500);

        dc.SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

        for (const auto& node : nodes) {
            wxColor elementColor(100, 100, 95);

            if (currentMorse == node.path) {
                elementColor = *wxGREEN;
            }
            else if (!currentMorse.empty() && node.path.size() >= currentMorse.size() &&
                node.path.compare(0, currentMorse.size(), currentMorse) == 0) {
                elementColor = *wxRED;
            }

            dc.SetBrush(wxBrush(elementColor));
            dc.SetPen(wxPen(elementColor));

            if (node.isCircle) {
                dc.DrawCircle(node.position, 14);
            }
            else {
                dc.DrawRectangle(node.position.x - 16, node.position.y - 10, 32, 20);
            }

            dc.SetTextForeground(*wxWHITE);
            wxString text(node.letter);
            wxSize textSize = dc.GetTextExtent(text);
            dc.DrawText(text, node.position.x - (textSize.x / 2), node.position.y - (textSize.y / 2));
        }

        dc.SetTextForeground(*wxWHITE);
        dc.SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
        dc.DrawText("Current Input: " + wxString(currentMorse), 25, 630);

        wxString label = "Decoded Text: ";
        wxString fullText = label + wxString(typedText);
        wxSize fullTextSize = dc.GetTextExtent(fullText);
        int maxAllowedWidth = 450;

        if (fullTextSize.x > maxAllowedWidth) {
            wxString visibleText = wxString(typedText);
            while (!visibleText.IsEmpty() && dc.GetTextExtent(label + visibleText).x > maxAllowedWidth) {
                visibleText.Remove(0, 1);
            }
            dc.DrawText(label + visibleText, 25, 665);
        }
        else {
            dc.DrawText(fullText, 25, 665);
        }
    }

    void DecodePending() {
        if (currentMorse.empty()) return;

        for (const auto& node : nodes) {
            if (node.path == currentMorse) {
                typedText += node.letter;
                break;
            }
        }

        currentMorse = "";
    }

    void RestartIdleTimers() {
        letterTimer.Stop();
        wordGapTimer.Stop();
        letterTimer.StartOnce(LetterGapMs);
        wordGapTimer.StartOnce(WordGapMs);
    }

    void OnReleaseTimer(wxTimerEvent& event) {
        dashBurstActive = false;
        RestartIdleTimers();
    }

    void OnLetterTimer(wxTimerEvent& event) {
        DecodePending();
        Refresh();
    }

    void OnWordGapTimer(wxTimerEvent& event) {
        typedText += " ";
        Refresh();
    }

    void OnKeyDown(wxKeyEvent& event) {
        int keyCode = event.GetKeyCode();

        if (keyCode == '.') {
            dashBurstActive = false;
            releaseTimer.Stop();

            if (currentMorse.length() < 7) currentMorse += ".";

            RestartIdleTimers();
            Refresh();
        }
        else if (keyCode == '-') {
            if (!dashBurstActive) {
                dashBurstActive = true;
                if (currentMorse.length() < 7) currentMorse += "-";
            }

            letterTimer.Stop();
            wordGapTimer.Stop();
            releaseTimer.StartOnce(SignalTimeoutMs);

            Refresh();
        }
        else if (keyCode == WXK_BACK) {
            releaseTimer.Stop();
            letterTimer.Stop();
            wordGapTimer.Stop();
            dashBurstActive = false;
            currentMorse = "";
            if (!typedText.empty()) typedText.pop_back();
            Refresh();
        }

        event.Skip();
    }
};

class MorseApp : public wxApp {
public:
    virtual bool OnInit() {
        wxImage::AddHandler(new wxPNGHandler);

        MorseWindow* window = new MorseWindow();
        window->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MorseApp);