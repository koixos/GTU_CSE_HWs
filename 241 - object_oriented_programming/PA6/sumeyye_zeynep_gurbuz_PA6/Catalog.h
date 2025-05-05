#ifndef CATALOG_H
#define CATALOG_H

#include <iostream>
#include <vector>
#include <string>
using std::vector;
using std::string;

const vector<string> CATALOG_LIST = {"book", "music", "movie"};

class Catalog {
    public:
        Catalog() { };
        Catalog(const string &_title, const string &_year) : title(_title), year(_year) { }
        const string getTitle() { return title; }
        const string getYear() { return year; }
        const bool getIsMissing() { return isMissing; }
        const void checkEntry(const vector<string> &data) { // checks if any entry is missing
            for (auto inf : data)
                if (inf.compare("NULL") == 0) 
                    this->isMissing = true;
        }
    private:
        string title = "NULL";
        string year = "NULL";    
        bool isMissing = false;
};

#endif