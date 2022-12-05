#include <wx/wx.h>
#include <wx/socket.h>
#include "components/imagePanel.h"

#include "utils.h"

#include <wx/listctrl.h>
#include <wx/colour.h>

// for testing
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <string>
using namespace std;

enum {
	WF_SOCKET
};

class ListingFrame : public wxFrame {
public:
	ListingFrame(const wxString& title, const wxPoint& pos, const wxSize& size, int n);
private:
    wxListView *listOfUser;
    void populateListView(int n);
};

