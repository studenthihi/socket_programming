#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "config.h"
using namespace std;


void load_questions(vector<string>& question_list, vector<string>& answer_list);
string serialize(string question, int id);
void random_M_questions(vector<string>& serialized_data, vector<string>& answer_list, int M);
