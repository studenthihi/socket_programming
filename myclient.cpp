// Client side C/C++ program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

using namespace std;
#define PORT 8080

int main(int argc, char const *argv[])
{
	int sock = 0, valread, client_fd;
	struct sockaddr_in serv_addr;
	char hello[] = "Hello from client";
	char buffer[1024] = {0};
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary
	// form
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if ((client_fd = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0){
		printf("\nConnection Failed \n");
		return -1;
	}

	memset(buffer, 0, sizeof(buffer));
	valread = recv(sock, buffer, 1024, 0);
	printf("%s\n", buffer);

	while (true) {
		// receive question from server
		memset(buffer, 0, sizeof(buffer));
		valread = recv(sock, buffer, 1024, 0);
		printf("%s\n", buffer);

		if (strcmp(buffer, "Wrong answer") == 0) {
			break;
		}
		if (strcmp(buffer, "You finished the quiz") == 0){
			break;
		}

		// send back answer
		string answer;
		getline(cin, answer);
		cout << answer << endl;
		send(sock, answer.c_str(), answer.length(), 0);
		cout << "Answer sent" << endl;
	}
	close(client_fd);
	return 0;
}
