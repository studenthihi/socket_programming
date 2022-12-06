#include "landing-frame.h"
#include "waiting-frame.h"

LandingFrame::LandingFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(NULL, wxID_ANY, title, pos, size), socketClient(NULL) {
	auto font = wxFONTFAMILY_MODERN;
	
	// Set background
	wxImagePanel* background = new wxImagePanel(this, wxT("background.jpeg"), wxBITMAP_TYPE_JPEG);
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	mainSizer->Add(background, 1, wxEXPAND);
	SetSizer(mainSizer);
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	
	// [1] Padding
	sizer->AddStretchSpacer();
	
	// [2] Game title and subtitle
	wxPanel* titlePanel = new wxPanel(background, wxID_ANY, wxDefaultPosition);
	wxStaticText* subtitleText = new wxStaticText(titlePanel, wxID_ANY, "Welcome to", wxDefaultPosition, wxDefaultSize, wxTE_CENTER);
	subtitleText->SetFont(wxFont(16, font, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	subtitleText->SetForegroundColour(wxColor(255, 255, 255));

	wxStaticText* titleText = new wxStaticText(titlePanel, wxID_ANY, "Who wants to be a millionaire?", wxDefaultPosition, wxDefaultSize, wxTE_CENTER);
	titleText->SetFont(wxFont(28, font, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_EXTRABOLD));
	titleText->SetForegroundColour(wxColor(255, 255, 255));

	wxBoxSizer* titleSizer = new wxBoxSizer(wxVERTICAL);
	titleSizer->AddStretchSpacer();
	titleSizer->Add(subtitleText, wxSizerFlags().Expand());
	titleSizer->Add(titleText, wxSizerFlags().Expand());
	titleSizer->AddStretchSpacer();
	titlePanel->SetSizer(titleSizer);
	sizer->Add(titlePanel, 1, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 60);

	sizer->AddSpacer(20);

	// [3] Input nickname
	wxStaticText* namePrompt = new wxStaticText(background, wxID_ANY, "Please input your nickname to start:", wxDefaultPosition, wxDefaultSize, wxTE_CENTER);
	namePrompt->SetFont(wxFont(12, font, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL));
	namePrompt->SetForegroundColour(wxColor(255, 255, 255));

	sizer->Add(namePrompt, 0, wxALIGN_CENTER | wxBOTTOM, 5);

	name = new wxTextCtrl(background, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(300, 40), wxTE_CENTER);
	name->SetFont(wxFont(16, font, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	sizer->Add(name, 0, wxALIGN_CENTER | wxBOTTOM, 30);

	wxButton* button = new wxButton(background, LD_ENTER_BUTTON, "Join game!");
	button->SetFont(wxFont(16, font, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_SEMIBOLD));
	sizer->Add(button, 0, wxALIGN_CENTER, 0);

	// [4] Padding
	sizer->AddStretchSpacer();
	
	background->SetSizer(sizer);

	// Connect socket
	socketClient = new wxSocketClient();
	socketClient->SetEventHandler(*this, LD_SOCKET);
	wxIPV4address adr;
	adr.Hostname(_T("localhost"));
	adr.Service(8080);
	socketClient->SetNotify(wxSOCKET_CONNECTION_FLAG | wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
	socketClient->Notify(true);

	if (socketClient->Connect(adr, false)) {
		wxMessageBox("Failed to connect to game server. Please try another time!");
		Close();
	}
}

void LandingFrame::OnEnter(wxCommandEvent& event) {
	nameStr = trim((name->GetValue()).ToStdString());
	int sz = nameStr.size();
	if (sz == 0) {
		wxMessageBox("Please enter a valid name!");
		return;
	}
	char* buffer = new char[sz + 2];
	buffer[0] = SendCode::ID_USER_NAME;
	for (int i = 0; i < sz; ++i) {
		buffer[i + 1] = nameStr[i];
	}
	buffer[sz + 1] = '\0';
	socketClient->Write(buffer, strlen(buffer));
	if (socketClient->GetLastIOWriteSize() != strlen(buffer)) {
		wxMessageBox("Failed to join game, try again! :(");
	}
}

void LandingFrame::OnSocket(wxSocketEvent& event) {
	wxSocketBase* sock = event.GetSocket();
	
	switch (event.GetSocketEvent()) {
	case wxSOCKET_CONNECTION: {
		break;
	}
	case wxSOCKET_INPUT: {
		char buffer[10];
		sock->Read(buffer, sizeof(buffer));
		if (sock->GetLastIOReadSize() > 0) {
			if (buffer[0] == ReceiveCode::ID_INVALID_NAME) {
				wxMessageBox("Invalid name, please don't use '$' in your name!");
			}
			else if (buffer[0] == ReceiveCode::ID_DUPLICATE_NAME) {
				wxMessageBox("Your name is a duplicate with another user, please try again!");
			}
			else if (buffer[0] == ReceiveCode::ID_ACCEPT_USER) {
				WaitingFrame* waitingFrame = new WaitingFrame("Waiting", GetPosition(), GetSize(), socketClient, false, nameStr);
				waitingFrame->Show(true);
				Close();
			}
			else if (buffer[0] == ReceiveCode::ID_REFUSE_USER) {
				wxMessageBox("The game room is full. Please try another time! :(");
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

std::string LandingFrame::trim(std::string s) {
	std::string t = "";
	int n = s.size();
	int i = 0, j = n - 1;
	while (i < n && s[i] == ' ') ++i;
	while (j > i && s[j] == ' ') --j;
	for (; i <= j; ++i) {
		t += s[i];
	}
	return t;
}

wxBEGIN_EVENT_TABLE(LandingFrame, wxFrame)
	EVT_BUTTON(LD_ENTER_BUTTON, LandingFrame::OnEnter)
	EVT_SOCKET(LD_SOCKET, LandingFrame::OnSocket)
wxEND_EVENT_TABLE()
