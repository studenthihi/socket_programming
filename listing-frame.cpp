#include "listing-frame.h"

// for testing
unordered_set<string> initlistPlayers() {
    unordered_set<string> playerNames;
    playerNames.insert("hihi");
    playerNames.insert("aaaa");
    playerNames.insert("bbbb");
    playerNames.insert("cccc");
    playerNames.insert("dddd");
    return playerNames;
}

ListingFrame::ListingFrame(const wxString& title, const wxPoint& pos, const wxSize& size, int n,
    int numQuestion, int order, vector<string> players, wxSocketClient* socketClient)
	: wxFrame(NULL, wxID_ANY, title, pos, size), socketClient(socketClient) {
	auto font = wxFONTFAMILY_MODERN;
	
	// Set background
	wxImagePanel* background = new wxImagePanel(this, wxT("background.jpeg"), wxBITMAP_TYPE_JPEG);
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	mainSizer->Add(background, 1, wxEXPAND);
	SetSizer(mainSizer);
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    
    // Padding
    sizer->AddStretchSpacer();

	// Notification field
    wxStaticText* titleText = new wxStaticText(background, wxID_ANY, "Game information", wxDefaultPosition, wxDefaultSize, wxTE_CENTER);
	titleText->SetFont(wxFont(28, font, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	wxStaticText* subtitleText = new wxStaticText(background, wxID_ANY, "There are " + to_string(n) + " players playing in order:", wxDefaultPosition, wxDefaultSize, wxTE_CENTER);
	subtitleText->SetFont(wxFont(14, font, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	
    sizer->Add(titleText, 0, wxALIGN_CENTER);
    sizer->AddSpacer(20);
	sizer->Add(subtitleText, 0, wxALIGN_CENTER | wxBOTTOM, 10);
    
    // List items field
    wxPanel *userPanel = new wxPanel(background, wxID_ANY, wxDefaultPosition);
    listOfUser = new wxListView(userPanel);
    listOfUser->AppendColumn("No.", wxLIST_FORMAT_CENTRE, 150);
    listOfUser->AppendColumn("Username", wxLIST_FORMAT_CENTRE, 320);
    listOfUser->AlwaysShowScrollbars(false, false);
    listOfUser->SetFont(wxFont(12, font, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

    wxBoxSizer* listSizer = new wxBoxSizer(wxVERTICAL);
    listSizer->Add(listOfUser, 1, wxALIGN_CENTRE_HORIZONTAL);
    userPanel->SetSizer(listSizer);
    populateListView(n, players);
    sizer->Add(userPanel, 0, wxALIGN_CENTER);

    // Notify the number of questions
    std::string question = "You are player number " + to_string(order) + "!\n There are " + to_string(numQuestion) + " questions in this set.";
    wxStaticText* numOfQuestion = new wxStaticText(background, wxID_ANY, question, wxDefaultPosition, wxDefaultSize, wxTE_CENTER);
	numOfQuestion->SetFont(wxFont(14, font, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    sizer->Add(numOfQuestion, 0, wxALIGN_CENTER | wxTOP, 10);

    sizer->AddStretchSpacer();

	background->SetSizer(sizer);

    socketClient->SetEventHandler(*this, LiF_SOCKET);
}

void ListingFrame::OnSocket(wxSocketEvent& event) {
	wxSocketBase* sock = event.GetSocket();

	switch (event.GetSocketEvent()) {
	case wxSOCKET_INPUT: {
		char buffer[512];
		sock->Read(buffer, sizeof(buffer));
		if (sock->GetLastIOReadSize() > 0) {
            if (buffer[0] == ReceiveCode::ID_START_GAME) {
                WaitingFrame* waitingFrame = new WaitingFrame("Waiting", GetPosition(), GetSize(), socketClient);
                waitingFrame->Show(true);
                Close();
            }
		}
		break;
	}
	case wxSOCKET_LOST: {
		wxMessageBox("Lost connection with server! :(");
		Close();
	}
	}
}

void ListingFrame::populateListView(int n, vector<string>& players) {
    for (int i = 0; i < players.size(); ++i) {
        listOfUser->InsertItem(i, std::to_string(i+1));
        listOfUser->SetItem(i, 1, players[i]);
    }
}

wxBEGIN_EVENT_TABLE(ListingFrame, wxFrame)
    EVT_SOCKET(LiF_SOCKET, ListingFrame::OnSocket)
wxEND_EVENT_TABLE()


//// Test this frame
//class MyApp : public wxApp
//{
//public:
//    virtual bool OnInit();
//};
//
//wxIMPLEMENT_APP(MyApp);
//
//bool MyApp::OnInit()
//{
//    wxInitAllImageHandlers(); // this is important to load image
//    ListingFrame *frame = new ListingFrame("Hello World", wxDefaultPosition, wxSize(800, 600), 3);
//    frame->Show(true);
//    return true;
//}