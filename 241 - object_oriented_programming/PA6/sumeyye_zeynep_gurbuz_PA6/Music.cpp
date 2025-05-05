#include "Music.h"
#include <list>
using std::list;

vector<string> Music::sort(vector<Music> &catalog, const string &sort, const vector<string> &rowData) {
    list<string> lst; // holds sorted info
    vector<string> temp; // holds created vector
    // according to the sort field sort all the entries
    if (sort.compare("title") == 0) {
        for (auto entry : catalog)
            lst.push_back(entry.getTitle());
        lst.sort();
        // sorting entries according to sorted list
        for (list<string>::iterator it = lst.begin(); it != lst.end(); it++)
            for (int i = 0; i < catalog.size(); i++)  
                if (catalog[i].getTitle().compare(*it) == 0)
                    temp.push_back(rowData[i]);
    }
    
    if (sort.compare("artists") == 0) {
        for (auto entry : catalog)
            lst.push_back(entry.getArtists());
        lst.sort();

        for (list<string>::iterator it = lst.begin(); it != lst.end(); it++)
            for (int i = 0; i < catalog.size(); i++)  
                if (catalog[i].getArtists().compare(*it) == 0)
                    temp.push_back(rowData[i]);
    }

    if (sort.compare("year") == 0) {
        for (auto entry : catalog)
            lst.push_back(entry.getYear());
        lst.sort();

        for (list<string>::iterator it = lst.begin(); it != lst.end(); it++)
            for (int i = 0; i < catalog.size(); i++)  
                if (catalog[i].getYear().compare(*it) == 0)
                    temp.push_back(rowData[i]);
    }
    
    if (sort.compare("genre") == 0) {
        for (auto entry : catalog)
            lst.push_back(entry.getGenre());
        lst.sort();

        for (list<string>::iterator it = lst.begin(); it != lst.end(); it++)
            for (int i = 0; i < catalog.size(); i++)  
                if (catalog[i].getGenre().compare(*it) == 0)
                    temp.push_back(rowData[i]);
    }
    return temp; // returning sorted entries to push the output vector
}

string Music::search(vector<Music> &catalog, const string &searchFor, const string &searchIn, const vector<string> &rowData) {
    string found = ""; // holds matching entries
    // according to given info and search field searches for all entries
    if (searchIn.compare("title") == 0) {
        for (int i = 0; i < catalog.size(); i++) {
            if (catalog[i].getTitle().find(searchFor) > 0
                && catalog[i].getTitle().find(searchFor) < catalog[i].getTitle().length()) {
                    found.append(rowData[i]);
                    found.append("\n");
            }    
        }
    }
    
    if (searchIn.compare("artists") == 0) {
        for (int i = 0; i < catalog.size(); i++) {
            if (catalog[i].getArtists().find(searchFor) > 0
                && catalog[i].getArtists().find(searchFor) < catalog[i].getArtists().length()) {
                    found.append(rowData[i]);
                    found.append("\n");
            }
        }
    }

    if (searchIn.compare("year") == 0) {
        for (int i = 0; i < catalog.size(); i++) {
            if (catalog[i].getYear().find(searchFor) > 0
                && catalog[i].getYear().find(searchFor) < catalog[i].getYear().length()) {
                    found.append(rowData[i]);
                    found.append("\n");
            }
        }
    }
    
    if (searchIn.compare("genre") == 0) {
        for (int i = 0; i < catalog.size(); i++) {
            if (catalog[i].getGenre().find(searchFor) > 0
                && catalog[i].getGenre().find(searchFor) < catalog[i].getGenre().length()) {
                    found.append(rowData[i]);
                    found.append("\n");
            }   
        }
    }
    return found; // returning found entries to push the output vector
}