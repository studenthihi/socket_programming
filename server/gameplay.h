#pragma once
#include <vector>
#include "config.h"
#include "utils.h"
using namespace std;

void mainGame(int client_socket[N]);

bool pingPlayer(int sd);
void announceCurrentPlayer(int client_socket[N], int current, int& outgame_count);
void announceWinner(int sd, int client_socket[N]);
void announceWA(int sd);
bool announceTA(int sd);
void allowMove(int sd);
void cleanup(int id, int client_socket[N], int& outgame_count);
