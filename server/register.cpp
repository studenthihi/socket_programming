#include "register.h"

unordered_map<int, string> findNPlayersSocketAndName(int masterSocket) {
	char buffer[BUFLEN + 1];
	char recvBuffer[BUFLEN + 1];

	cout << "Starting a new game. Waiting for users to join...\n";

	fd_set readfds;
	unordered_set<int> candidateSocket;
	unordered_set<string> playerNames;
	unordered_map<int, string> sd2PlayerName;

	int playerCount = 0;

	// Look for N players with distinct names.
	while (true) {
		FD_ZERO(&readfds);
		FD_SET(masterSocket, &readfds);

		int max_sd = masterSocket;
		for (int sd : candidateSocket) {
			FD_SET(sd, &readfds);
			if (sd > max_sd)
				max_sd = sd;
		}

		int activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
		if (activity < 0 && errno != EINTR) {
			cout << "Network error...\n";
		}

		if (FD_ISSET(masterSocket, &readfds)) {
			int newSocket = accept(masterSocket, NULL, NULL);
			if (newSocket < 0) {
				cout << "Network error, game server ending...\n";
				close(masterSocket);
				exit(1);
			}
			cout << "New candidate connected with socket " << newSocket << ", waiting for name\n";
			candidateSocket.insert(newSocket);
		}
		unordered_set<int> removedSocket;
		for (int sd : candidateSocket) {
			if (FD_ISSET(sd, &readfds)) {
				int valread = recv(sd, buffer, BUFLEN, 0);
				if (valread < 0) {
					cout << "Client socket " << sd << " has disconnected\n";
					close(sd);
					if (sd2PlayerName.find(sd) != sd2PlayerName.end()) {
						playerNames.erase(sd2PlayerName[sd]);
						sd2PlayerName.erase(sd);
						playerCount -= 1;
					}
					removedSocket.insert(sd); // Cannot erase immediately from candidateSocket bc we're traversing it
				}
				else if (valread > 0) {
					buffer[valread] = '\0';
					if (buffer[0] == ReceiveCode::ID_USER_NAME) {
						string name(buffer + 1);
						cout << "Socket " << sd << " sent name " << name << "\n";
						if (playerNames.find(name) == playerNames.end()) {
							// Valid name, accept player
							buffer[0] = SendCode::ID_ACCEPT_USER;
							buffer[1] = '\0';
							if (send(sd, buffer, strlen(buffer), 0) < 0) {
								cout << "Socket error when sending to socket " << sd << "\n";
								close(sd);
								close(masterSocket);
								exit(1);
							}
							playerNames.insert(name);
							sd2PlayerName[sd] = name; // do not remove sd of candidateSocket, to see if player disconnect before starting
							playerCount += 1;
							if (playerCount == N) { // Enough player
								break;
							}
						}
						else {
							buffer[0] = SendCode::ID_INVALID_NAME;
							buffer[1] = '\0';
							if (send(sd, buffer, strlen(buffer), 0) < 0) {
								cout << "Socket error when sending to socket " << sd << "\n";
								close(sd);
								close(masterSocket);
								exit(1);
							}
						}
					}
				}
			}
		}
		for (int sd : removedSocket)
			candidateSocket.erase(sd);

		if (playerCount == N) break;
	}

	// We have enough N players, now we can refuse all remaining candidate and start the game

	for (int sd : candidateSocket) { // Refuse them
		if (sd2PlayerName.find(sd) != sd2PlayerName.end())
			continue; // already a player, do not refuse
		buffer[0] = SendCode::ID_REFUSE_USER;
		buffer[1] = '\0';
		if (send(sd, buffer, strlen(buffer), 0) < 0) {
			cout << "Socket error when sending to socket " << sd << "\n";
			close(sd);
			close(masterSocket);
			exit(1);
		}
		close(sd);
	}

	return sd2PlayerName;
}