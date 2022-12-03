#include <wx/wx.h>
#include"components/imagePanel.cpp"
using namespace std;

class InGameFrame:public wxFrame{
    public:
        InGameFrame(const wxString &title, const wxPoint &pos, const wxSize &size);
    private:
        void OnSize(wxSizeEvent &);
        void OnClick(wxCommandEvent &);
        wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(InGameFrame, wxFrame)
EVT_BUTTON(wxID_ANY, InGameFrame::OnClick)
EVT_SIZE(InGameFrame::OnSize)
wxEND_EVENT_TABLE()

void InGameFrame::OnSize(wxSizeEvent &e)
{
    cout << "FRAME OnSize" << endl;
    e.Skip();
}

void InGameFrame::OnClick(wxCommandEvent &e)
{
    cout << "FRAME OnClick, id = " << e.GetId() << endl;
    e.Skip();
}

InGameFrame::InGameFrame(const wxString &title, const wxPoint &pos, const wxSize &size) : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    wxImagePanel* panel_top = new wxImagePanel(this, wxT("background.jpeg"), wxBITMAP_TYPE_JPEG);
    wxPanel *panel_bottom = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(800, 200));
    panel_bottom->SetBackgroundColour(wxColor(20, 6, 59));

    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(panel_top, 2, wxEXPAND | wxLEFT | wxTOP | wxRIGHT, 0); 
    sizer->Add(panel_bottom, 1, wxEXPAND | wxALL, 0);

    // ========= Top Panel =========
    // ====== Header ======
    wxPanel* header = new wxPanel(panel_top, wxID_ANY, wxDefaultPosition, wxSize(800, 40));
    wxButton *btn_move = new wxButton(header, wxID_ANY, "Move", wxPoint(0, 0), wxSize(60, 40));
    wxButton *btn_quit = new wxButton(header, wxID_ANY, "Quit", wxPoint(0, 0), wxSize(60, 40));
    wxBoxSizer *header_sizer = new wxBoxSizer(wxHORIZONTAL);
    header_sizer->Add(btn_move, 0, wxEXPAND | wxALL, 10);
    header_sizer->Add(btn_quit, 0, wxEXPAND | wxALL, 10);
    header->SetSizerAndFit(header_sizer);

    // ====== Question Box ======
    wxPanel* pseudoText = new wxPanel(panel_top, wxID_ANY, wxDefaultPosition,
                                  wxSize(600, 50));
                                //   wxBORDER_THEME|wxTAB_TRAVERSAL);
    wxTextCtrl* text = new wxTextCtrl(pseudoText, wxID_ANY, "Question 1: Who is the president of United State?",
                                    wxDefaultPosition, wxDefaultSize,
                                    wxTE_CENTRE|wxBORDER_NONE);
    text->SetBackgroundColour(wxColor(20, 6, 59));
    text->SetForegroundColour(wxColor(255, 255, 255));
    text->SetFont(wxFont(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    pseudoText->SetBackgroundColour(wxColor(20, 6, 59));
    wxBoxSizer* szr = new wxBoxSizer(wxVERTICAL);
    szr->AddStretchSpacer();
    szr->Add(text, wxSizerFlags().Expand());
    szr->AddStretchSpacer();
    pseudoText->SetSizer(szr);

    wxBoxSizer *sizer_top = new wxBoxSizer(wxVERTICAL);
    sizer_top->Add(header, 0, wxEXPAND | wxBOTTOM, 50);
    // sizer_top->Add(question, 1, wxEXPAND | wxALL, 30);
    sizer_top->Add(pseudoText, 1, wxEXPAND | wxALL, 30);
    panel_top->SetSizer(sizer_top);

    // ========= Bottom Panel =========
    wxPanel *column1 = new wxPanel(panel_bottom, wxID_ANY, wxDefaultPosition, wxSize(200, 200));
    wxPanel *column2 = new wxPanel(panel_bottom, wxID_ANY, wxDefaultPosition, wxSize(200, 200));

    wxBoxSizer* sizer_bottom = new wxBoxSizer(wxHORIZONTAL);
    sizer_bottom->Add(column1, 1, wxEXPAND | wxALL, 0);
    sizer_bottom->Add(column2, 1, wxEXPAND | wxALL, 0);
    panel_bottom->SetSizerAndFit(sizer_bottom);

    
    wxBoxSizer* sizer_column1 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_column2 = new wxBoxSizer(wxVERTICAL);


    wxButton* btn_answer11 = new wxButton(column1, wxID_ANY, wxT("A. Thao Huyen"), wxPoint(200,200), wxDefaultSize, 0);
    wxButton* btn_answer12 = new wxButton(column1, wxID_ANY, wxT("B. Thao Huyen"), wxPoint(200,200), wxDefaultSize, 0);
    sizer_column1-> Add(btn_answer11,  1, wxEXPAND | wxALL, 10);
    sizer_column1-> Add(btn_answer12,  1, wxEXPAND | wxALL, 10);
    column1->SetSizerAndFit(sizer_column1);

    wxButton* btn_answer21 = new wxButton(column2, wxID_ANY, wxT("C. Thao Huyen"), wxPoint(200,200), wxDefaultSize, 0);
    wxButton* btn_answer22 = new wxButton(column2, wxID_ANY, wxT("D. Thao Huyen"), wxPoint(200,200), wxDefaultSize, 0);
    sizer_column2 -> Add(btn_answer21,  1, wxEXPAND | wxALL, 10);
    sizer_column2 -> Add(btn_answer22,  1, wxEXPAND | wxALL, 10);
    column2-> SetSizerAndFit(sizer_column2);

    this->SetSizer(sizer);
}



// Test this frame
class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    wxInitAllImageHandlers(); // this is important to load image
    InGameFrame *frame = new InGameFrame("Hello World", wxDefaultPosition, wxSize(800, 600));
    frame->Show(true);
    return true;
}