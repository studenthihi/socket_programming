#include "waiting-frame.h"

WaitingFrame::WaitingFrame(const wxString& title, const wxPoint& pos, const wxSize& size, std::string name)
	: wxFrame(NULL, wxID_ANY, title, pos, size), name(name) {
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
}