#define _CRT_SECURE_NO_WARNINGS

#include "landing-frame.h"

class MyApp : public wxApp {
public:
	bool OnInit() {
		wxInitAllImageHandlers();
		LandingFrame* landing = new LandingFrame("Client", wxPoint(50, 50), wxSize(800, 600));
		landing->Show(true);
		return true;
	}
};

//wxIMPLEMENT_APP(MyApp);
