#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include"const.h"
using namespace std;

#ifndef UTILS_H
#define UTILS_H

void load_questions(vector<string> &question_list, vector<vector<string>> &options, vector<string> &answer_list)
{
    ifstream fin;
    fin.open("data.txt");
    if (!fin.is_open())
    {
        cout << "Error opening file" << endl;
        exit(EXIT_FAILURE);
    }
    string line;
    getline(fin, line); // skip the first line 
    while (getline(fin, line))
    {
        question_list.push_back(line);
        vector<string> option;
        for (int i = 0; i < 4; i++)
        {
            getline(fin, line);
            option.push_back(line);
        }
        options.push_back(option);
        getline(fin, line);
        answer_list.push_back(line);
    }
    fin.close();
}


string serialize(string question, vector<string> options, int id)
{
    // format: "<CODE><NO> <QUESTION>|<OPTION1>|<OPTION2>|<OPTION3>|<OPTION4>"
    string serialized_data;
    // serialized_data += TRUE_ANSWER;
    // serialized_data += (to_string(id) + " " + question);
    // serialized_data += options[0];
    // serialized_data += options[1];
    // serialized_data += options[2];
    // serialized_data += options[3];

    for (int i = 0; i < options.size(); i++)
    {   
        // serialized_data += "|\0";
        serialized_data.append(options[i]);
    }
    // for (int i = 0; i < 4; i++)
    // {
    //     serialized_data += (" , " + options[i]);
    // }

    cout << serialized_data << endl;
    return serialized_data;
}

void random_M_questions(vector<string> serialized_data, vector<string> &answer_list, int M)
{   
    vector<string> question_list;
    vector<vector<string>> options;
    load_questions(question_list, options, answer_list);
    srand(time(NULL));
    int n = question_list.size();
    for (int i = 0; i < M; i++)
    {
        int j = rand() % n;
        swap(question_list[i], question_list[j]);
        swap(options[i], options[j]);
        serialized_data.push_back(serialize(question_list[i], options[i], i));
        swap(answer_list[i], answer_list[j]);
    }
}



#endif
