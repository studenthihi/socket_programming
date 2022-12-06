#pragma once
#include<wx/wx.h>
#include<string>
#include<vector>
#include<cstring>
#include"components/imagePanel.h"
#include <wx/socket.h>
#include <wx/timer.h>
#include"utils.h"

using namespace std;

enum ButtonId
{
    move_id = 5,
    quit_id = 6,
    option1_id = 1, 
    option2_id = 2,
    option3_id = 3,
    option4_id = 4
};

enum {
    IG_SOCKET = 7,
    IG_TIMER = 8
};

const wxString TIME_LIMIT= "15";

class InGameFrame : public wxFrame{
    public:
        InGameFrame(const wxString &title, const wxPoint &pos, const wxSize &size, 
            wxSocketClient* socketClient, string first_question);
    private:
        void OnSize(wxSizeEvent &);
        void OnClick(wxCommandEvent &);
        void OnSocket(wxSocketEvent &e);
        void OnUpdateTimer(wxTimerEvent &e);
        
        
        // frame components
        wxButton* btn_answer11;
        wxButton* btn_answer12;
        wxButton* btn_answer21;
        wxButton* btn_answer22;
        wxTextCtrl* text; // question

        // timer
        wxTimer* timer;
        wxStaticText* timer_display;
        
        // socket
        wxSocketClient* socketClient;
        string last_message;

        // support function
        // int analyse_message(vector<string> &data);
        void deserialize(char* text, vector<string>&data );

        wxDECLARE_EVENT_TABLE();
};
