#pragma once
#include <wx/wx.h>
#include "components/imagePanel.h"

class WaitingFrame : public wxFrame {
public:
	WaitingFrame(const wxString& title, const wxPoint& pos, const wxSize& size, std::string name);
private:
	std::string name;
};
