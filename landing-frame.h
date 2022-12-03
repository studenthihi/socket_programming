#pragma once
#include <wx/wx.h>
#include <wx/textctrl.h>
#include <string>
#include "components/imagePanel.h"
#include "waiting-frame.h"

enum {
	LD_ENTER_BUTTON	
};

class LandingFrame : public wxFrame {
public:
	LandingFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
private:
	wxTextCtrl* name;
	void OnEnter(wxCommandEvent& event);
	void OnAllowedEnter();
	void OnRejectEnter();

	bool validRegister(std::string name);

	wxDECLARE_EVENT_TABLE();
};