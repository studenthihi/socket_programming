#include"ingame-frame.h"

InGameFrame::InGameFrame(const wxString &title, const wxPoint &pos, const wxSize &size, wxSocketClient* socket, string first_question) : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    // ===== Load data ======
    this->socketClient = socket;
    this->last_message = first_question;

    // // Tell server that client is ready to play
    // socketClient->Write("Give me question", strlen("Give me question"));
    // vector<string> data;

    analyse_message(data);

    if (data.size() == 0){
        cout << "Error: Cannot load question" << endl;
        Close();
    }

    // ===== Create two main panels =====
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
    text = new wxTextCtrl(pseudoText, wxID_ANY, "Question " + data[1],
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


    btn_answer11 = new wxButton(column1, option1_id, wxT("A. " + data[2]), wxPoint(200,200), wxDefaultSize, 0);
    btn_answer12 = new wxButton(column1, option2_id, wxT("B. " + data[3]), wxPoint(200,200), wxDefaultSize, 0);
    sizer_column1-> Add(btn_answer11,  1, wxEXPAND | wxALL, 10);
    sizer_column1-> Add(btn_answer12,  1, wxEXPAND | wxALL, 10);
    column1->SetSizerAndFit(sizer_column1);

    btn_answer21 = new wxButton(column2, option3_id, wxT("C. " + data[4]), wxPoint(200,200), wxDefaultSize, 0);
    btn_answer22 = new wxButton(column2, option4_id, wxT("D. " + data[5]), wxPoint(200,200), wxDefaultSize, 0);
    sizer_column2 -> Add(btn_answer21,  1, wxEXPAND | wxALL, 10);
    sizer_column2 -> Add(btn_answer22,  1, wxEXPAND | wxALL, 10);
    column2-> SetSizerAndFit(sizer_column2);

    this->SetSizer(sizer);

    //Init socket
    // socketClient = new wxSocketClient();
	// socketClient->SetEventHandler(*this, IG_SOCKET);
	// wxIPV4address adr;
	// adr.Hostname(_T("localhost"));
	// adr.Service(8080);
	// socketClient->SetNotify(wxSOCKET_CONNECTION_FLAG | wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
	// socketClient->Notify(true);

	// if (socketClient->Connect(adr, false)) {
	// 	// wxMessageBox("Failed to connect to game server. Please try another time!");
	// 	// Close();
    //     cout << "Failed to connect to game server. Please try another time!" << endl;
	// }

    // cout << "Message received: " << last_message << endl;
}

// ================== Event Handler ==================
void InGameFrame::OnSocket(wxSocketEvent& event)
{
    wxSocketBase *sock = event.GetSocket();
    switch(event.GetSocketEvent())
    {
        case wxSOCKET_INPUT:
        {
            // Read the data from the socket
            char buffer[1024];
            sock->Read(buffer, 1024);
            wxString str(buffer, wxConvUTF8);
            
            this->last_message = buffer;
            cout << "Message saved: "<< last_message << endl;
            break;
        }
        case wxSOCKET_LOST:
        {
            // wxMessageBox("Connection lost");
            break;
        }
        default:
            break;
    }
}
void InGameFrame::OnSize(wxSizeEvent &e)
{
    // cout << "FRAME OnSize" << endl;
    e.Skip();
}

void InGameFrame::OnClick(wxCommandEvent &e)
{
    cout << "FRAME OnClick, id = " << e.GetId() << endl;
    // e.Skip();
    char buffer[1024];
    int id = e.GetId();
    if (id == move_id){
        cout << "Click move button"<< endl;
        buffer[0] = MOVE;
        buffer[1] = '\0';
        socketClient->Write(buffer, 1024);
        // Todo: move to waiting frame
    }

    else if (id == option1_id | id == option2_id | id == option3_id | id == option4_id){
        cout << "Answer" << endl;
        char answer[2];
        answer[0] = ANSWER;
        if (id == option1_id) answer[1] = 'A';
        else if (id == option2_id) answer[1] = 'B';
        else if (id == option3_id) answer[1] = 'C';
        else answer[1] ='D';

        socketClient->Write(answer, 2); // send answer to server
        vector<string> data; // receive result from server, if true server will send next question
        int code = analyse_message(data);

        if (code = TRUE_ANSWER){
            text->ChangeValue("Question " + data[1]);
            btn_answer11->SetLabel("A. " + data[2]);
            btn_answer12->SetLabel("B. " + data[3]);
            btn_answer21->SetLabel("C. " + data[4]);
            btn_answer22->SetLabel("D. " + data[5]);
        }
        else if (code == WRONG_ANSWER){
            // wxMessageBox("Oops... You answered wrong", "Win");
            // Close();
        }
        else if (code == WIN_GAME){
            // wxMessageBox("Congratulation! You win the game", "Win");
            // Close();
        }

        // TODO win or lose
    }
}



// ================== Utility function to receive data from server ====================
int InGameFrame::analyse_message(vector<string> &data){
    char buffer[1024];
    strcpy(buffer, last_message.c_str());
 
    if (strlen(buffer) == 0) return -1;

    if (buffer[0] == TRUE_ANSWER){
        deserialize(buffer, data);
        return TRUE_ANSWER;
    }
    else if (buffer[0] == WRONG_ANSWER){
        cout << "Wrong answer :<<<<" << endl;
        return WRONG_ANSWER;
    }
    else if (buffer[0] == WIN_GAME){
        cout << "Game win :>>>>>" << endl;
        return WIN_GAME;
    }
    return 0;
}

void InGameFrame::deserialize(char *text, vector<string> &data){
    cout <<text << endl;
    char *ptr; // declare a ptr pointer  
    ptr = strtok(text, "|"); 
    while (ptr != NULL)  
    {  
        data.push_back(ptr);
        ptr = strtok (NULL, "|");  
    }  
}


wxBEGIN_EVENT_TABLE(InGameFrame, wxFrame)
    EVT_BUTTON(wxID_ANY, InGameFrame::OnClick)
    EVT_SIZE(InGameFrame::OnSize)
    EVT_SOCKET(IG_SOCKET, InGameFrame::OnSocket)
wxEND_EVENT_TABLE()