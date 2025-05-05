#include "Functions.h"

void Functions::readFile(const string &fileName, vector<string> &vec) {
    char chr;
    string line;
    std::fstream inpStr;
    inpStr.open(fileName, std::ios::in);
    while (inpStr.get(chr)) { // reading file char by char
        if (chr == 10) { // a new line is detected
            vec.push_back(line); // push the current line into the list
            line.clear(); // empty the line
        } else {
            line.push_back(chr); // still on the same line 
        }
    }
    inpStr.close();
    vec.push_back(line); // pushing the last line
}

void Functions::writeFile(const string &fileName, vector<string> &vec) {
    std::fstream outStr;
    outStr.open(fileName, std::ios::out);
    for (int i = 0; i < vec.size(); i++)
        outStr << vec[i]; // pushing each line
    outStr.close();
}

vector<string> Functions::analyseData(const string &line) {
    vector<string> entry;
    for (int i = 0; i < line.length(); i++) {
        string word;
        int indStart = line.find('"', i), // find start index of info
            indEnd = line.find('"', indStart+1); // find end index of info
        if (indStart >= 0 && indStart < indEnd && indEnd > indStart && indEnd < line.length()) // if found indexes in range of line length
            for (int j = indStart; j <= indEnd; j++) // read the word between quotes
                word.push_back(line[j]);
        entry.push_back(word); // push each word into info string
        i = indEnd+1; // moving cursor to the end of current info
    }
    return entry;
}

vector<string> Functions::analyseCommands(const string &line) {
    vector<string> command;
    int cursor = 0; // cursor at the start of the line
    int ind = Functions::isValid(COMMAND_LIST, line); // checks if command is valid
    if (ind == -1) // unvalid command
        return {"NULL"};

    cursor = COMMAND_LIST[ind].length(); // where the cursor is left
    command.push_back(COMMAND_LIST[ind]);
    
    if (cursor >= line.length() || line[++cursor] != '"') // if the field to sort is missing
        return {"NULL"};

    if (command[0].compare("sort") == 0) {   
        string word = Functions::findQuotes(cursor+1, line.length(), line);
        if (word.compare("NULL") == 0) 
            return {"NULL"}; // if command is missing
        command.push_back(word); 
        return command; // command is valid
    }
    
    if (command[0].compare("search") == 0) {
        int ind = line.find("in");
        if (ind <= cursor+2 || ind >= line.length()) // if the field to search or "in" keyword is missing
            return {"NULL"};
        
        string word = Functions::findQuotes(cursor+1, ind-1, line);
        if (word.compare("NULL") == 0)
            return {"NULL"}; // if command is missing
        
        command.push_back(word);
        command.push_back("in");
        cursor = ind+1;

        if ((++cursor) >= line.length() || line[++cursor] != '"') // if the field to search is missing
            return {"NULL"};

        word = Functions::findQuotes(cursor+1, line.length(), line);
        if (word.compare("NULL") == 0) // if command is missing
            return {"NULL"};

        command.push_back(word);
        return command; // command is valid
    }
    return {"NULL"}; // default
}

string Functions::findQuotes(int start, int end, const string &line) {
    string word;
    for (int i = start; i < end; i++) {   
        if (line[i] != '"' && i == end-1 || line[i] == '"' && i != end-1) 
            return "NULL"; // if the sort field is missing or extra info is given
      
        if (line[i] == '"' && i == end-1) // if command is valid
            return word;
        
        word.push_back(line[i]);
    }
    return "NULL"; // default
}