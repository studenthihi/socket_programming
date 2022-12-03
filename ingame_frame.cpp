#include<wx/wx.h>
#include<string>
#include<vector>
#include"components/imagePanel.cpp"

using namespace std;

#include"utils.cpp"

enum ButtonId
{
    move_id = 5,
    quit_id = 6,
    option1_id = 1, 
    option2_id = 2,
    option3_id = 3,
    option4_id = 4
};

class InGameFrame:public wxFrame{
    public:
        InGameFrame(const wxString &title, const wxPoint &pos, const wxSize &size);
    private:
        void OnSize(wxSizeEvent &);
        void OnClick(wxCommandEvent &);
        wxDECLARE_EVENT_TABLE();

        // frame components
        wxButton* btn_answer11;
        wxButton* btn_answer12;
        wxButton* btn_answer21;
        wxButton* btn_answer22;
        wxTextCtrl* text; // question

        // question set
        vector<string> question_list;
        vector<vector<string>> options;
        vector<int> answer_list;
        int cur_question = 0;

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
    // e.Skip();
    int id = e.GetId();
    if (id == move_id){
        cout << "Click move button"<< endl;
        wxFrame* frame = new wxFrame(NULL, wxID_ANY, wxT("Hello World"), wxPoint(50,50), wxSize(800,600));
        frame->Show();
    }

    else if (id == option1_id | id == option2_id | id == option3_id | id == option4_id){
        cout << "Answer" << endl;
        int answer = 0;
        if (id == option1_id) answer = 0;
        else if (id == option2_id) answer = 1;
        else if (id == option3_id) answer = 2;
        else answer = 3;

        if (answer == answer_list[cur_question]){
            cout << "Answer Right";
            cur_question += 1;
            if (cur_question == question_list.size()){
                cout << "Win" << endl;
                e.Skip();
            }
            else {
                text->ChangeValue("Question " + to_string(cur_question + 1) + ": " + question_list[cur_question]);
                btn_answer11->SetLabel("A. " + options[cur_question][0]);
                btn_answer12->SetLabel("B. " + options[cur_question][1]);
                btn_answer21->SetLabel("C. " + options[cur_question][2]);
                btn_answer22->SetLabel("D. " + options[cur_question][3]);
            }
        } 
        else {
            cout << "Answer Wrong" << endl;
            e.Skip();
        }
    }
}

InGameFrame::InGameFrame(const wxString &title, const wxPoint &pos, const wxSize &size) : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    // ===== Load data ======
    question_list = {"who is the president of VN?", "Which term is used to describe the legal profession in the USA?"};
    options = {{"France", "Italy", "Spain", "Greece"}, 
            {"The Chair", "The Stand", "The Robe", "The Bar"}};
    answer_list = {1, 2};

    string question = "Question 1: " +  question_list[0];

    wxImagePanel* panel_top = new wxImagePanel(this, wxT("background.jpeg"), wxBITMAP_TYPE_JPEG);
    wxPanel *panel_bottom = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(800, 200));
    panel_bottom->SetBackgroundColour(wxColor(20, 6, 59));

    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(panel_top, 2, wxEXPAND | wxLEFT | wxTOP | wxRIGHT, 0); 
    sizer->Add(panel_bottom, 1, wxEXPAND | wxALL, 0);


    // ========= Top Panel =========
    // ====== Header ======
    wxPanel* header = new wxPanel(panel_top, wxID_ANY, wxDefaultPosition, wxSize(800, 40));
    wxButton *btn_move = new wxButton(header, move_id, "Move", wxPoint(0, 0), wxSize(60, 40));
    wxButton *btn_quit = new wxButton(header, wxID_ANY, "Quit", wxPoint(0, 0), wxSize(60, 40));
    wxBoxSizer *header_sizer = new wxBoxSizer(wxHORIZONTAL);
    header_sizer->Add(btn_move, 0, wxEXPAND | wxALL, 10);
    header_sizer->Add(btn_quit, 0, wxEXPAND | wxALL, 10);
    header->SetSizerAndFit(header_sizer);

    // ====== Question Box ======
    wxPanel* pseudoText = new wxPanel(panel_top, wxID_ANY, wxDefaultPosition,
                                  wxSize(600, 50));
                                //   wxBORDER_THEME|wxTAB_TRAVERSAL);
    text = new wxTextCtrl(pseudoText, wxID_ANY, question,
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


    btn_answer11 = new wxButton(column1, option1_id, wxT("A. " + options[0][0]), wxPoint(200,200), wxDefaultSize, 0);
    btn_answer12 = new wxButton(column1, option2_id, wxT("B. " + options[0][1]), wxPoint(200,200), wxDefaultSize, 0);
    sizer_column1-> Add(btn_answer11,  1, wxEXPAND | wxALL, 10);
    sizer_column1-> Add(btn_answer12,  1, wxEXPAND | wxALL, 10);
    column1->SetSizerAndFit(sizer_column1);

    btn_answer21 = new wxButton(column2, option3_id, wxT("C. " + options[0][2]), wxPoint(200,200), wxDefaultSize, 0);
    btn_answer22 = new wxButton(column2, option4_id, wxT("D. " + options[0][3]), wxPoint(200,200), wxDefaultSize, 0);
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