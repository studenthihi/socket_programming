#include "gameplay.h"

void mainGame(int client_socket[N]) {
	vector<string> serialized_questions;
	vector<string> answer;
	random_M_questions(serialized_questions, answer, num_question);

	int id = 0; // current client
	int cur_question_index = 0;
	int outgame_count = 0;
	int moved[N]; // check if client allowed to move
	memset(moved, 0, sizeof(moved));

	char buffer[BUFLEN + 1];
	int socket, valread;

	while (true) {
		// [Pre-checking a few things before sending question]
		if (outgame_count == N) {
			cout << "All players have lost or disconnected...\n";
			return;
		}
		socket = client_socket[id];
		if (socket == 0) {
			id = (id + 1) % N;
			continue;
		}
		if (!pingPlayer(socket)) {
			cleanup(id, client_socket, outgame_count);
			id = (id + 1) % N;
			continue;
		}
		announceCurrentPlayer(client_socket, id, outgame_count);
		if (outgame_count == N - 1) {
			announceWinner(id, client_socket);
			return;
		}

		// [Sending question]
		string question = serialized_questions[cur_question_index];
		if (!moved[id]) {
			question += "|1";
		}
		else {
			question += "|0";
		}
		if (send(socket, question.c_str(), question.length(), 0) != question.length()) {
			cout << "Player socket " << socket << " disconnected...\n";
			cleanup(id, client_socket, outgame_count);
			id = (id + 1) % N;
			continue;
		}

		// [Set timer]
		Timer timer(socket);
		timer.start(15, chrono::milliseconds(1000), [](int s) {
			cout << "Time out" << endl;
			char buffer[2];
			buffer[0] = SendCode::ID_TIME_OUT;
			buffer[1] = '\0';
			if (send(s, buffer ,2 , 0) != 2)
				perror("send");
			sleep(1);
			close(s);
		});


		// [Waiting for answer]
		memset(buffer, 0, sizeof(buffer));
		valread = recv(socket, buffer, BUFLEN, 0);

		// [Stop timer]
		timer.stop();

		if (valread < 0) {
			cleanup(id, client_socket, outgame_count);
			id = (id + 1) % N;
			continue;
		}
		if (valread > 0) {
			buffer[valread] = '\0';
			if (buffer[0] == ReceiveCode::ID_ANSWER) {
				if (cur_question_index == num_question - 1) {
					announceWinner(id, client_socket);
					return;
				}
				if (buffer[1] == answer[cur_question_index][0]) {
					cur_question_index += 1;
					continue;
				}
				announceWA(socket);
				sleep(1);
				cleanup(id, client_socket, outgame_count);
				id = (id + 1) % N;
				continue;
			}
			else if (buffer[0] == ReceiveCode::ID_MOVE) {
				if (!moved[id]) {
					moved[id] = 1;
					allowMove(socket);
					sleep(1);
				}
				else {
					announceWA(socket);
					cleanup(id, client_socket, outgame_count);
				}
				id = (id + 1) % N;
				continue;
			}
		}
	}
}

bool pingPlayer(int sd) {
	char buffer[2];
	buffer[0] = SendCode::ID_PING;
	buffer[1] = '\0';

	if (send(sd, buffer, strlen(buffer), 0) < 0) {
		return false;
	}
	if (recv(sd, buffer, sizeof(buffer), 0) < 0) {
		return false;
	}
	return true;
}

void announceCurrentPlayer(int client_socket[N], int current, int& outgame_count) {
	char buffer[3];
	buffer[0] = SendCode::ID_CUR_PLAYER;
	buffer[1] = current + 1;
	buffer[2] = '\0';

	for (int i = 0; i < N; ++i) {
		if (i != current && client_socket[i] > 0) {
			if (send(client_socket[i], buffer, strlen(buffer), 0) < 0) {
				cleanup(i, client_socket, outgame_count);
			}
		}
	}
}

void announceWinner(int id, int client_socket[N]) {
	char buffer[3];
	for (int i = 0; i < N; ++i) {
		if (i != id && client_socket[i] > 0) {
			buffer[0] = SendCode::ID_WIN_ANNOUNCE;
			buffer[1] = id + 1;
			buffer[2] = '\0';
			send(client_socket[i], buffer, strlen(buffer), 0);
		}
		else if (i == id) {
			buffer[0] = SendCode::ID_WIN_GAME;
			buffer[1] = '\0';
			send(client_socket[i], buffer, strlen(buffer), 0);
		}
	}
}

void announceWA(int sd) {
	char buffer[2];
	buffer[0] = SendCode::ID_WRONG_ANSWER;
	buffer[1] = '\0';
	send(sd, buffer, strlen(buffer), 0);
}

bool announceTA(int sd) {
	char buffer[2];
	buffer[0] = SendCode::ID_TRUE_ANSWER;
	buffer[1] = '\0';
	if (send(sd, buffer, strlen(buffer), 0) < 0) {
		return false;
	}
	return true;
}

void allowMove(int sd) {
	char buffer[2];
	buffer[0] = SendCode::ID_ALLOWED_MOVE;
	buffer[1] = '\0';
	send(sd, buffer, strlen(buffer), 0);
}

void cleanup(int id, int client_socket[N], int& outgame_count) {
	//close(client_socket[id]);
	client_socket[id] = 0;
	++outgame_count;
}
