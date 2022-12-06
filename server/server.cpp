#if defined(_WIN64)
	#pragma comment (lib, "Ws2_32.lib")
	#define _CRT_SECURE_NO_WARNINGS
#endif

#include "setup.h"
#include "register.h"
#include "prepare.h"
#include "gameplay.h"
using namespace std;

int main() {
	int master_socket = setupMasterSocket();
	
	// Play game after game.
	while (true) {
		unordered_map<int, string> sd2PlayerName = findNPlayersSocketAndName(master_socket);

		int client_socket[N];
		int ci = 0;
		for (auto p : sd2PlayerName) {
			client_socket[ci++] = p.first;
		}
		sleep(1);

		// Prepare game
		prepareGame(sd2PlayerName, client_socket, master_socket);

		// Let's play
		mainGame(client_socket);
	}
	return 0;
}