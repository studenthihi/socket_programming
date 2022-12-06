#pragma once

#include <iostream>
#include <unordered_map>
#include "config.h"
using namespace std;

void prepareGame(unordered_map<int, string>& sd2PlayerName, int client_socket[N], int master_socket);
