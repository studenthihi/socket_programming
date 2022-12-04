#pragma once
#include <wx/wx.h>
#include <wx/textctrl.h>
#include <wx/socket.h>
#include <string>
#include "components/imagePanel.h"
#include "waiting-frame.h"
#include "utils.h"

enum {
	LD_ENTER_BUTTON,
	LD_SOCKET
};

class LandingFrame : public wxFrame {
public:
	LandingFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
private:
	wxTextCtrl* name;
	std::string nameStr;
	wxSocketClient* socketClient;
	void OnEnter(wxCommandEvent& event);
	void OnSocket(wxSocketEvent& event);

	std::string trim(std::string s);

	wxDECLARE_EVENT_TABLE();
};