#ifndef MUSIC_H
#define MUSIC_H

#include "Catalog.h"

const int MUSIC_INFO_NUM = 4;
const vector<string> VARIABLES_MUSIC = {"title", "artists", "year", "genre"};

class Music : public Catalog {
    public:
        Music() : Catalog() { };
        Music(const vector<string> &data)
            : Catalog(data[0], data[2]), artists(data[1]), genre(data[3]) { this->checkEntry(data); }
        static vector<string> sort(vector<Music> &, const string &, const vector<string> &);
        static string search(vector<Music> &, const string &, const string &, const vector<string> &);
        const string getArtists() { return artists; }
        const string getGenre() { return genre; }
        static string getType() { return CATALOG_LIST[1]; }
        static int getVariableNum() { return MUSIC_INFO_NUM; }
        static bool checkVariableType(const string& checkFor) { // checks if given variable type is valid
            for (auto var : VARIABLES_MUSIC)
                if (var.compare(checkFor) == 0)
                    return true;
            return false;
        }
    private:
        string artists = "NULL";
        string genre = "NULL";
};

#endif