#include "listing-frame.h"
#include "waiting-frame.h"
#include "ingame-frame.h"

WaitingFrame::WaitingFrame(const wxString& title, const wxPoint& pos, const wxSize& size, 
	wxSocketClient* socketClient, bool inGame, std::string name)
	: wxFrame(NULL, wxID_ANY, title, pos, size), name(name), socketClient(socketClient) {
	auto font = wxFONTFAMILY_MODERN;

	// Set background
	wxImagePanel* background = new wxImagePanel(this, wxT("background.jpeg"), wxBITMAP_TYPE_JPEG);
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	mainSizer->Add(background, 1, wxEXPAND);
	SetSizer(mainSizer);
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	// Text
	sizer->AddStretchSpacer();
	text = new wxStaticText(background, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_CENTER);
	text->SetFont(wxFont(16, font, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	if (inGame) {
		setTextByState(IN_GAME_UNKNOWN);
	}
	else {
		setTextByState(OUT_GAME);
	}
	sizer->Add(text, 0, wxALIGN_CENTER);
	sizer->AddStretchSpacer();

	background->SetSizer(sizer);

	socketClient->SetEventHandler(*this, WF_SOCKET);
}

void WaitingFrame::OnSocket(wxSocketEvent& event) {
	wxSocketBase* sock = event.GetSocket();

	switch (event.GetSocketEvent()) {
	case wxSOCKET_INPUT: {
		char buffer[512];
		sock->Read(buffer, sizeof(buffer));
		int sz;
		if ((sz = sock->GetLastIOReadSize()) > 0) {
			buffer[sz] = '\0';
			if (buffer[0] == ReceiveCode::ID_GAME_INFO && sz > 4) {
				int n = buffer[1];
				int numQuestion = buffer[2];
				int order = buffer[3];
				string names(buffer + 4);
				vector<string> players = parsePlayers(names);

				ListingFrame* frame = new ListingFrame(
					"Game info", GetPosition(), GetSize(), 
					n, numQuestion, order, players, socketClient);

				frame->Show(true);
				Close();
			}
			else if (buffer[0] == ReceiveCode::ID_PING) {
				char sendBuff[2];
				sendBuff[0] = SendCode::ID_PING_ANS;
				sendBuff[1] = '\0';
				sock->Write(sendBuff, strlen(sendBuff));
			}
			else if (buffer[0] == ReceiveCode::ID_CUR_PLAYER && sz > 1) {
				setTextByState(IN_GAME, buffer[1]);
			}
			else if (buffer[0] == ReceiveCode::ID_WIN_GAME) {
				wxMessageBox("Congratulations! You win the game because all other players are either lost or disconnected.");
				Close();
			}
			else if (buffer[0] == ReceiveCode::ID_WIN_ANNOUNCE && sz > 1) {
				wxMessageBox("Oh oh! Player " + to_string(buffer[1]) + " won the game. Try again next time! :(");
				Close();
			}
			else if (buffer[0] == ReceiveCode::ID_QUESTION) {
				InGameFrame* ingame = new InGameFrame("Playing", GetPosition(), GetSize(), socketClient, buffer);
				ingame->Show(true);
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

void WaitingFrame::setTextByState(WaitingState state, int currentPlayer) {
	std::string info;
	if (state == OUT_GAME) {
		info = "Welcome " + name + "!\nYou joined successfully.\nWaiting for others to join...";
	}
	else if (state == IN_GAME_UNKNOWN) {
		info = "Game has started.\nPlease wait for your turn...";
	}
	else {
		info = "Game in progress.\nPlayer " + std::to_string(currentPlayer) + " is playing...";
	}
	text->SetLabelText(info);
}

vector<string> WaitingFrame::parsePlayers(string& s) {
	vector<string> players;
	int i = 0;
	while (i < s.size()) {
		string player = "";
		while (i < s.size() && s[i] != '$') {
			player += s[i++];
		}
		players.push_back(player);
		++i;
	}
	return players;
}

wxBEGIN_EVENT_TABLE(WaitingFrame, wxFrame)
	EVT_SOCKET(WF_SOCKET, WaitingFrame::OnSocket)
wxEND_EVENT_TABLE()
