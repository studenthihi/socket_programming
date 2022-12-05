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

ListingFrame::ListingFrame(const wxString& title, const wxPoint& pos, const wxSize& size, int n)
	: wxFrame(NULL, wxID_ANY, title, pos, size) {
	auto font = wxFONTFAMILY_MODERN;
	
	// Set background
	// wxImagePanel* background = new wxImagePanel(this, wxT("background.jpeg"), wxBITMAP_TYPE_JPEG);
    wxPanel* background = new wxPanel(this);
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	mainSizer->Add(background, 1, wxEXPAND);
	SetSizer(mainSizer);
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    
    // Padding
    sizer->AddStretchSpacer();

	// Notification field
    wxPanel* notiPanel = new wxPanel(background, wxID_ANY, wxDefaultPosition);
    wxStaticText* titleText = new wxStaticText(notiPanel, wxID_ANY, "Registration Completed Successfully", wxDefaultPosition, wxDefaultSize, wxTE_CENTER);
	titleText->SetFont(wxFont(28, font, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	wxStaticText* subtitleText = new wxStaticText(notiPanel, wxID_ANY, "Here is the order of players", wxDefaultPosition, wxDefaultSize, wxTE_CENTER);
	subtitleText->SetFont(wxFont(16, font, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	
    wxBoxSizer* notiSizer = new wxBoxSizer(wxVERTICAL);
	notiSizer->Add(titleText, wxSizerFlags().Expand());
    notiSizer->AddSpacer(30);
	notiSizer->Add(subtitleText, wxSizerFlags().Expand());
    notiPanel->SetSizer(notiSizer);
    sizer->Add(notiPanel, 1, wxEXPAND | wxLEFT | wxRIGHT);

    // Padding
    sizer->AddStretchSpacer();

    // List items field
    wxPanel *userPanel = new wxPanel(background, wxID_ANY, wxDefaultPosition);
    listOfUser = new wxListView(userPanel);
    listOfUser->AppendColumn("No.", wxLIST_FORMAT_CENTRE, 150);
    listOfUser->AppendColumn("Username", wxLIST_FORMAT_CENTRE, 320);
    listOfUser->SetBackgroundColour(wxTransparentColor);
    listOfUser->AlwaysShowScrollbars(false, false);
    
    wxBoxSizer* listSizer = new wxBoxSizer(wxVERTICAL);
    listSizer->Add(listOfUser, 1, wxALIGN_CENTRE_HORIZONTAL);
    userPanel->SetSizer(listSizer);
    populateListView(n);
    sizer->Add(userPanel, 1, wxEXPAND | wxLEFT | wxRIGHT);

    // Padding
    sizer->AddStretchSpacer();

    // Notify the number of questions
    std::string question = "Number of question " + std::to_string(5*n);
    wxStaticText* numOfQuestion = new wxStaticText(background, wxID_ANY, question, wxDefaultPosition, wxDefaultSize, wxTE_CENTER);
	numOfQuestion->SetFont(wxFont(16, font, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    sizer->Add(numOfQuestion, 0, wxEXPAND | wxBOTTOM, 60);

	background->SetSizer(sizer);

}

void ListingFrame::populateListView(int n) {
    unordered_set<string> playerNames = initlistPlayers();
    int i = 0;
    unordered_set<string> :: iterator itr;
    for (itr = playerNames.begin(); itr != playerNames.end() && i < n; itr++, i++) {
        listOfUser->InsertItem(i, std::to_string(i+1));
        listOfUser->SetItem(i, 1, (*itr));
    }
}

// Test this frame
class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    wxInitAllImageHandlers(); // this is important to load image
    ListingFrame *frame = new ListingFrame("Hello World", wxDefaultPosition, wxSize(800, 600), 3);
    frame->Show(true);
    return true;
}