#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using std::vector;
using std::string;

const vector<string> COMMAND_LIST = {"search", "sort"};

class Functions { // general functions
    public:
        static void readFile(const string &, vector<string> &);
        static void writeFile(const string &, vector<string> &);
        static vector<string> analyseData(const string &); // analyses read data & extracts info
        static vector<string> analyseCommands(const string &); // analyses read commands & extracts info
        static string findQuotes(int start, int end, const string &); // finds quoted infos
        static int isValid(const vector<string> &vec, const string &type) { // checks if a catalog/command is valid
            for (int i = 0; i < vec.size(); i++)
                if (type.find(vec[i]) == 0)
                    return i;
            return -1;
        }
};

#endif