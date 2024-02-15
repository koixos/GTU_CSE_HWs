#ifndef MOVIE_H
#define MOVIE_H

#include "Catalog.h"

const int MOVIE_INFO_NUM = 5;
const vector<string> VARIABLES_MOVIE = {"title", "director", "year", "genre", "starring"};

class Movie : public Catalog {
    public:
        Movie() : Catalog() { };
        Movie(const vector<string> &data)
            : Catalog(data[0], data[2]), director(data[1]), genre(data[3]), starring(data[4]) { this->checkEntry(data); }
        static vector<string> sort(vector<Movie> &, const string &, const vector<string> &);
        static string search(vector<Movie> &, const string &, const string &, const vector<string> &);
        const string getDirector() { return director; }
        const string getGenre() { return genre; }
        const string getStarring() { return starring; }
        static string getType() { return CATALOG_LIST[2]; }
        static int getVariableNum() { return MOVIE_INFO_NUM; }
        static bool checkVariableType(const string& checkFor) { // checks if given variable type is valid
            for (auto var : VARIABLES_MOVIE)
                if (var.compare(checkFor) == 0)
                    return true;
            return false;
        }
    private:
        string director = "NULL";
        string genre = "NULL";
        string starring = "NULL";
};

#endif