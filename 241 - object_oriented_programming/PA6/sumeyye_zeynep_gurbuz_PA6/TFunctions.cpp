#include <iostream>
#include <vector>
#include <string>
using std::vector;
using std::string;

template <typename T>
class TFunctions { // template functions to operate on different classes
    public:
        static vector<T> createEntries(vector<vector<string>> &);
        static vector<string> process(vector<T> &, vector<vector<string>> &, const vector<string> &, const vector<string> &);
};

template <typename T>
vector<T> TFunctions<T>::createEntries(vector<vector<string>> &data) {
    vector<T> vec;
    for (auto entry : data) {
        if (entry.size() < T::getVariableNum()) // if any info is missing
            entry.push_back("NULL");
        vec.push_back(T(entry));
    }
    return vec;
}

template <typename T>
vector<string> TFunctions<T>::process(vector<T> &catalog, vector<vector<string>> &commands, const vector<string> &rowData, const vector<string> &rowCommands) {
    vector<T> temp; // holds new data vector
    vector<string> output, row; // holds row output

    output.push_back("Catalog Read: ");
    output.push_back(T::getType());

    bool isValid[catalog.size()]; // holds if given entries valid or not
    for (int i = 0; i < catalog.size(); i++)
        isValid[i] = true; // initializing

    for (int i = 0; i < catalog.size(); i++) {
        bool doesExist = false;
        for (int j = 0; j < temp.size(); j++) {
            if (catalog[i].getTitle().compare(temp[j].getTitle()) == 0) {
                doesExist = true; // duplicate found
                break;
            }
        }

        if (!doesExist) { // if entry is unique
            if (catalog[i].getIsMissing()) { // if any info is missing
                output.push_back("\nException: missing field\n");
                output.push_back(rowData[i+1]);
                isValid[i] = false;
            } else { // if entry is valid
                temp.push_back(catalog[i]);
                row.push_back(rowData[i+1]);
            }
        } else { // if duplicate found
            if (catalog[i].getIsMissing()) // if any info is missing
                output.push_back("\nException: missing field\n");
            else
                output.push_back("\nException: duplicate entry\n");

            output.push_back(rowData[i+1]);
            isValid[i] = false;
        }
    }

    int counter = 0;
    for (auto val : isValid)
        if (val)
            ++counter; // calculating valid & unique entries
    output.push_back("\n");
    output.push_back(std::to_string(counter));
    output.push_back(" unique entries\n");
    // COMMANDS PART
    for (int i = 0; i < commands.size(); i++) {
        if (commands[i][0].compare("sort") == 0) { // if sort command is used
            if (!T::checkVariableType(commands[i][1])) { // if given field is unvalid
                output.push_back("Exception: command is wrong\n");
                output.push_back(rowCommands[i]);
            } else {
                output.push_back(rowCommands[i]);
                output.push_back("\n");
                vector<string> vec = T::sort(temp, commands[i][1], row);
                for (auto val : vec) {
                    output.push_back(val);
                    output.push_back("\n");
                }
            }
        } else if (commands[i][0].compare("search") == 0) { // if search command is used
            if (!T::checkVariableType(commands[i][3])) { // if given field is unvalid
                output.push_back("Exception: command is wrong\n"); 
                output.push_back(rowCommands[i]);
                output.push_back("\n");
            } else {
                output.push_back(rowCommands[i]);
                output.push_back("\n");
                output.push_back(T::search(temp, commands[i][1], commands[i][3], row));
            }
        } else { // if command is unvalid
            output.push_back("Exception: command is wrong\n");
            output.push_back(rowCommands[i]);
        }
    }
    return output;
}