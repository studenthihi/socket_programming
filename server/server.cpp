#if defined(_WIN64)
	#pragma comment (lib, "Ws2_32.lib")
#endif

#include "setup.h"
#include "register.h"

int main() {
	char buffer[BUFLEN + 1];
	
	int master_socket = setupMasterSocket();
	
	// Play game after game.
	while (true) {
		unordered_map<int, string> sd2PlayerName = findNPlayersSocketAndName(master_socket);

		int client_socket[N];
		int ci = 0;
		for (auto p : sd2PlayerName) {
			client_socket[ci++] = p.first;
		}

		sleep(2);

		// Startup game
		buffer[0] = SendCode::ID_START_GAME; // todo (VanMai): add more info...
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

		// Let's play
		
	}
	return 0;
}