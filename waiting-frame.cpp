#include "waiting-frame.h"

WaitingFrame::WaitingFrame(const wxString& title, const wxPoint& pos, const wxSize& size, std::string name, wxSocketClient* socketClient)
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
	std::string info = "Welcome " + name + "!\nYou joined successfully.\nWaiting for others to join...";
	wxStaticText* text = new wxStaticText(background, wxID_ANY, info, wxDefaultPosition, wxDefaultSize, wxTE_CENTER);
	text->SetFont(wxFont(16, font, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	sizer->Add(text, 0, wxALIGN_CENTER);
	sizer->AddStretchSpacer();

	background->SetSizer(sizer);

	socketClient->SetEventHandler(*this, WF_SOCKET);
}

void WaitingFrame::OnSocket(wxSocketEvent& event) {
	wxSocketBase* sock = event.GetSocket();

	switch (event.GetSocketEvent()) {
	case wxSOCKET_INPUT: {
		char buffer[10];
		sock->Read(buffer, sizeof(buffer));
		if (sock->GetLastIOReadSize() > 0) {
			if (buffer[0] == ReceiveCode::ID_START_GAME) {
				wxMessageBox("The game has started!");
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

wxBEGIN_EVENT_TABLE(WaitingFrame, wxFrame)
	EVT_SOCKET(WF_SOCKET, WaitingFrame::OnSocket)
wxEND_EVENT_TABLE()
