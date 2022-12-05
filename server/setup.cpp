#include "setup.h"

#if defined(_WIN64)
int setupMasterSocket() {
	WSADATA d;
	if (WSAStartup(MAKEWORD(2, 2), &d)) {
		cout << "Failed to initialize game server.\n";
		exit(1);
	}

	addrinfo* result = NULL, hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(NULL, PORT, &hints, &result)) {
		cout << "Failed to get address for game server.\n";
		WSACleanup();
		exit(1);
	}

	int masterSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (masterSocket == INVALID_SOCKET) {
		cout << "Failed to create socket for game server\n";
		WSACleanup();
		exit(1);
	}

	if (bind(masterSocket, result->ai_addr, (int)result->ai_addrlen) < 0) {
		cout << "Failed to bind socket for game server.\n";
		freeaddrinfo(result);
		closesocket(masterSocket);
		WSACleanup();
		exit(1);
	}

	cout << "Game server started.\n";
	freeaddrinfo(result);

	if (listen(masterSocket, MAX_SOCKET) < 0) {
		cout << "Failed to listen to player clients.\n";
		closesocket(masterSocket);
		WSACleanup();
		exit(1);
	}

	return masterSocket;
}
#else

int setupMasterSocket() {
	int master_socket;
	int opt = 1;
	struct sockaddr_in address;

	//create a master socket
	if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("Failed to create socket for game server.");
		exit(EXIT_FAILURE);
	}

	//set master socket to allow multiple connections ,
	//this is just a good habit, it will work without this
	if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt,
		sizeof(opt)) < 0)
	{
		perror("Failed to set socket option for game server.");
		exit(EXIT_FAILURE);
	}

	//type of socket created
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// mynote: htons: converts 32-bit (4-byte) quantities from host byte order to network byte order

	//bind the socket to localhost port 8888
	if (bind(master_socket, (struct sockaddr*)&address, sizeof(address)) < 0)
	{
		perror("Failed to bind socket for game server.");
		exit(EXIT_FAILURE);
	}

	//try to specify maximum of 3 pending connections for the master socket
	if (listen(master_socket, 3) < 0)
	{
		perror("Failed to listen for players.");
		exit(EXIT_FAILURE);
	}

	return master_socket;
}

#endif