#include<iostream>
#include<string>
#include<fstream>
#include<vector>
using namespace std;

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

void random_questions(vector<string> &question_list, vector<vector<string>> &options, vector<string> &answer_list)
{
    srand(time(NULL));
    int n = question_list.size();
    for (int i = 0; i < n; i++)
    {
        int j = rand() % n;
        swap(question_list[i], question_list[j]);
        swap(options[i], options[j]);
        swap(answer_list[i], answer_list[j]);
    }
}


