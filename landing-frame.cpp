#include "landing-frame.h"

LandingFrame::LandingFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(NULL, wxID_ANY, title, pos, size) {
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
	wxStaticText* titleText = new wxStaticText(titlePanel, wxID_ANY, "Who wants to be a millionaire?", wxDefaultPosition, wxDefaultSize, wxTE_CENTER);
	titleText->SetFont(wxFont(28, font, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_EXTRABOLD));
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
	sizer->Add(namePrompt, 0, wxALIGN_CENTER | wxBOTTOM, 5);

	name = new wxTextCtrl(background, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(300, 40), wxTE_CENTER);
	name->SetFont(wxFont(16, font, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	sizer->Add(name, 0, wxALIGN_CENTER | wxBOTTOM, 30);

	wxButton* button = new wxButton(background, LD_ENTER_BUTTON, "Join game!");
	button->SetFont(wxFont(16, font, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_SEMIBOLD));
	sizer->Add(button, 0, wxALIGN_RIGHT | wxRIGHT, 60);

	// [4] Padding
	sizer->AddStretchSpacer();
	
	background->SetSizer(sizer);
}

void LandingFrame::OnEnter(wxCommandEvent& event) {
	std::string nameStr = (name->GetValue()).ToStdString();
	if (validRegister(nameStr)) {
		WaitingFrame* waitingFrame = new WaitingFrame("Waiting", GetPosition(), GetSize(), nameStr);
		waitingFrame->Show(true);
		Close();
	}
	else {
		wxMessageBox("Invalid name, please try another one!");
	}
}

bool LandingFrame::validRegister(std::string name) {
	return name.size() % 2 == 0;
}

wxBEGIN_EVENT_TABLE(LandingFrame, wxFrame)
	EVT_BUTTON(LD_ENTER_BUTTON, LandingFrame::OnEnter)
wxEND_EVENT_TABLE()
