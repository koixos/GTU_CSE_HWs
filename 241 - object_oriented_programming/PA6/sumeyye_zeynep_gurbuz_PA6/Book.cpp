#include "Book.h"
#include <list>
using std::list;

vector<string> Book::sort(vector<Book> &catalog, const string &sort, const vector<string> &rowData) {
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
    
    if (sort.compare("authors") == 0) {
        for (auto entry : catalog)
            lst.push_back(entry.getAuthors());
        lst.sort();

        for (list<string>::iterator it = lst.begin(); it != lst.end(); it++)
            for (int i = 0; i < catalog.size(); i++)  
                if (catalog[i].getAuthors().compare(*it) == 0)
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
    
    if (sort.compare("tags") == 0) {
        for (auto entry : catalog)
            lst.push_back(entry.getTags());
        lst.sort();

        for (list<string>::iterator it = lst.begin(); it != lst.end(); it++)
            for (int i = 0; i < catalog.size(); i++)  
                if (catalog[i].getTags().compare(*it) == 0)
                    temp.push_back(rowData[i]);
    }
    return temp; // returning sorted entries to push the output vector
}

string Book::search(vector<Book> &catalog, const string &searchFor, const string &searchIn, const vector<string> &rowData) {
    string found = ""; // holds matching entries
    // according to given info and search field searches for all entries
    if (searchIn.compare("title") == 0) {
        for (int i = 0; i < catalog.size(); i++) { // if a match exists
            if (catalog[i].getTitle().find(searchFor) > 0
                && catalog[i].getTitle().find(searchFor) < catalog[i].getTitle().length()) {
                    found.append(rowData[i]);
                    found.append("\n");
            }    
        }
    }
    
    if (searchIn.compare("authors") == 0) {
        for (int i = 0; i < catalog.size(); i++) {
            if (catalog[i].getAuthors().find(searchFor) > 0
                && catalog[i].getAuthors().find(searchFor) < catalog[i].getAuthors().length()) {
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
    
    if (searchIn.compare("tags") == 0) {
        for (int i = 0; i < catalog.size(); i++) {
            if (catalog[i].getTags().find(searchFor) > 0
                && catalog[i].getTags().find(searchFor) < catalog[i].getTags().length()) {
                    found.append(rowData[i]);
                    found.append("\n");
            }   
        }
    }
    return found; // returning found entries to push the output vector
}