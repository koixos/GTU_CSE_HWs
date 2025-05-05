#ifndef BOOK_H
#define BOOK_H

#include "Catalog.h"

const int BOOK_INFO_NUM = 4;
const vector<string> VARIABLES_BOOK = {"title", "authors", "year", "tags"};

class Book : public Catalog {
    public:
        Book() : Catalog() { };
        Book(const vector<string> &data)
            : Catalog(data[0], data[2]), authors(data[1]), tags(data[3]) { this->checkEntry(data); }
        static vector<string> sort(vector<Book> &, const string &, const vector<string> &);
        static string search(vector<Book> &, const string &, const string &, const vector<string> &);
        const string getAuthors() { return authors; }
        const string getTags() { return tags; }
        static string getType() { return CATALOG_LIST[0]; }
        static int getVariableNum() { return BOOK_INFO_NUM; }
        static bool checkVariableType(const string& checkFor) { // checks if given variable type is valid
            for (auto var : VARIABLES_BOOK)
                if (var.compare(checkFor) == 0)
                    return true;
            return false;
        }
    private:
        string authors = "NULL";
        string tags = "NULL";
};

#endif