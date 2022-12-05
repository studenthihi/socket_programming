#include "prepare.h"

void prepareGame(unordered_map<int, string>& sd2PlayerName, int client_socket[N], int master_socket) {
	char buffer[BUFLEN + 1];

	// Send game information to all players.
	buffer[0] = SendCode::ID_GAME_INFO;
	buffer[1] = N;
	buffer[2] = num_question;
	buffer[3] = '-'; // buffer[3] will be the order of the corresponding player
	string playerNames = "";
	for (int i = 0; i < N; ++i) {
		playerNames += sd2PlayerName[client_socket[i]] + "$";
	}
	for (int i = 0; i < playerNames.size(); ++i) {
		buffer[i + 4] = playerNames[i];
	}
	buffer[playerNames.size() + 4] = '\0';
	for (int i = 0; i < N; ++i) {
		int sd = client_socket[i];
		buffer[3] = i + 1;
		if (send(sd, buffer, strlen(buffer), 0) < 0) {
			cout << "Socket error when sending to socket " << sd << "\n";
			close(sd);
			close(master_socket);
			exit(1);
		}
	}

	// Wait for players to read before starting game.
	sleep(8);

	// Send start game to all players
	cout << "Starting game...\n";
	buffer[0] = SendCode::ID_START_GAME;
	buffer[1] = '\0';
	for (int i = 0; i < N; ++i) {
		int sd = client_socket[i];
		if (send(sd, buffer, strlen(buffer), 0) < 0) {
			cout << "Socket error when sending to socket " << sd << "\n";
			close(sd);
			close(master_socket);
			exit(1);
		}
	}
	sleep(1);
}