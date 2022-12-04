#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

#include "wx/socket.h"
#include<wx/wx.h>
#include<string>
#include<vector>
#include"ingame_frame.cpp"



using namespace std;
#define PORT 8080


int create_socket(int& sock, int& client_fd); 

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp);


bool MyApp::OnInit()
{
    // create socket
    int socket, client_fd;
    create_socket(socket, client_fd);

    // TODO: show the order list of players

    // TODO: receive message notify it's client's turn


    // start game
    char buffer[1024] = {0};
    int valread;
    memset(buffer, 0, sizeof(buffer));
    valread = recv(socket, buffer, 1024, 0);
    printf("%s\n", buffer);


    wxInitAllImageHandlers(); // this is important to load image
    InGameFrame *frame = new InGameFrame("Hello World", wxDefaultPosition, wxSize(800, 600), socket);
    frame->Show(true);

    close(client_fd);
    return true;
}

int create_socket(int& sock, int& client_fd) {
    // int sock = 0, valread, client_fd;
	struct sockaddr_in serv_addr;
	char buffer[1024] = {0};
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary form
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if ((client_fd = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0){
		printf("\nConnection Failed \n");
		return -1;
	}
    return sock;
}