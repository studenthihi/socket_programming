#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include"const.h"
using namespace std;

#ifndef UTILS_H
#define UTILS_H

void load_questions(vector<string> &question_list, vector<string> &answer_list)
{
    ifstream fin;
    fin.open("new_data.txt");
    if (!fin.is_open())
    {
        cout << "Error opening file" << endl;
        exit(EXIT_FAILURE);
    }
    string line;
    while (getline(fin, line))
    {   
        question_list.push_back(line);
        getline(fin, line);
        answer_list.push_back(line);
    }
    fin.close();
}


string serialize(string question, int id)
{
    // format: "<CODE>|<NO> <QUESTION>|<OPTION1>|<OPTION2>|<OPTION3>|<OPTION4>"
    string serialized_data;
    serialized_data += TRUE_ANSWER;
    serialized_data += '|';
    serialized_data += (to_string(id) + ": " + question);

    // cout << serialized_data << endl;
    return serialized_data;
}

void random_M_questions(vector<string>& serialized_data, vector<string> &answer_list, int M)
{   
    vector<string> question_list;
    // vector<vector<string>> options;
    load_questions(question_list, answer_list);
    int n = question_list.size();
    for (int i = 0; i < M; i++)
    {
        int j = rand() % n;
        swap(question_list[i], question_list[j]);
        serialized_data.push_back(serialize(question_list[i], i+1));
        swap(answer_list[i], answer_list[j]);
    }
}



#endif
