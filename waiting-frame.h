#pragma once
#include <wx/wx.h>
#include <wx/socket.h>
#include "components/imagePanel.h"
#include <iostream>
#include "utils.h"
using namespace std;

enum {
	WF_SOCKET
};

class WaitingFrame : public wxFrame {
public:
	WaitingFrame(const wxString& title, const wxPoint& pos, const wxSize& size, wxSocketClient* socketClient, bool inGame = true, std::string name = "");
private:
	enum WaitingState {
		OUT_GAME,
		IN_GAME_UNKNOWN,
		IN_GAME
	};
	void OnSocket(wxSocketEvent& event);

	std::string name;
	wxSocketClient* socketClient;
	wxStaticText* text;
	vector<string> parsePlayers(string& s);
	
	void setTextByState(WaitingState state, int currentPlayer = -1);

	wxDECLARE_EVENT_TABLE();
};
