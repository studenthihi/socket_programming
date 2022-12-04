#pragma once

#if defined(_WIN64)
	#include <winsock2.h>
	#include <ws2tcpip.h>
	#define PORT "8080"
	#define close(x) closesocket(x) 
	#define sleep(x) Sleep(x * 1000)
#else
	#include <stdio.h>
	#include <string.h> //strlen
	#include <stdlib.h>
	#include <errno.h>
	#include <unistd.h> //close
	#include <arpa/inet.h> //close
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
	#define PORT 8080
#endif

#define BUFLEN 512

const int N = 2; // maximum number of clients
const int MAX_SOCKET = 3;

enum SendCode {
	ID_ACCEPT_USER = 5,
	ID_REFUSE_USER = 6,
	ID_INVALID_NAME = 7,
	ID_START_GAME = 8,
};
enum ReceiveCode {
	ID_USER_NAME = 5,
};