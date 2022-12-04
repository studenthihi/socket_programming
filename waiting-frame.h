#pragma once
#include <wx/wx.h>
#include <wx/socket.h>
#include "components/imagePanel.h"
#include "utils.h"

enum {
	WF_SOCKET
};

class WaitingFrame : public wxFrame {
public:
	WaitingFrame(const wxString& title, const wxPoint& pos, const wxSize& size, std::string name, wxSocketClient* socketClient);
private:
	void OnSocket(wxSocketEvent& event);

	std::string name;
	wxSocketClient* socketClient;

	wxDECLARE_EVENT_TABLE();
};
