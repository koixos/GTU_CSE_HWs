#include "Catalog.h"
#include "Book.h"
#include "Music.h"
#include "Movie.h"
#include "Functions.h"
#include "TFunctions.cpp"

const char *DATA = "data.txt";
const char *COMMANDS = "commands.txt";
const char *OUTPUT = "output.txt";

class Error {
    public:
        Error() {
            std::cout << "Error: Program terminated" << std::endl;
            exit(0);
        }
};

int main() {
    vector<string> rowData, rowCommands;
    Functions::readFile(DATA, rowData);
    Functions::readFile(COMMANDS, rowCommands);

    vector<vector<string>> data, commands; // hold processed info
    vector<string> output; // holds final output
    
    for (auto line : rowCommands) // analysing command by command
        commands.push_back(Functions::analyseCommands(line));

    for (int i = 1; i < rowData.size(); i++) // analysing entry by entry
        data.push_back(Functions::analyseData(rowData[i]));

    int type = Functions::isValid(CATALOG_LIST, rowData[0]); // checking given catalog type
    if (type == 0) {
        vector<Book> catalog = TFunctions<Book>::createEntries(data);
        output = TFunctions<Book>::process(catalog, commands, rowData, rowCommands);
    } else if (type == 1) {
        vector<Music> catalog = TFunctions<Music>::createEntries(data);
        output = TFunctions<Music>::process(catalog, commands, rowData, rowCommands);
    } else if (type == 2) {
        vector<Movie> catalog = TFunctions<Movie>::createEntries(data);
        output = TFunctions<Movie>::process(catalog, commands, rowData, rowCommands);
    } else {
        throw Error();
    } 

    Functions::writeFile(OUTPUT, output);
    
    return 0;
}