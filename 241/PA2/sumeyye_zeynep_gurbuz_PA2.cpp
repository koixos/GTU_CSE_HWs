#include <iostream>
#include <fstream>
#include <vector>
#include <cmath> // Only 'pow()' funct will be used in 'toInt()'

using namespace std;

// defining max & min values for rgb values
const int MAX_VALUE = 255;
const int MIN_VALUE = 0;

class Menu {
    public:
        Menu() { };
        void showMainMenu(); // Function to show the main menu
        void showSubmenu(int); // Function to show the chosen submenu
        int makeChoice(bool); // Function to make choice
        int checkChoice(string, bool); // Function to validate a given input
        long toInt(string); // Function to turn a string to an integer
};

class Image {
    private:
        vector<int> pixels; // Holds rgb values of image
        int row; // Holds row value of image
        int column; // Holds column value of image
    public:
        Image() { }
        vector<int> getPixels() { return pixels; }
        void setPixels(int); // Setter to push a value to the vector
        void setPixels(float, int); // Setter to push a value to a spesific index of the vector
        int getRow() { return row; }
        void setRow(int _row) { this->row = _row; }
        int getColumn() { return column; }
        void setColumn(int _column) { this->column = _column; }
};

class Editor {
    private:
        Menu menu;
        Image img;
    public:
        Editor() { }
        void run(); // Function to run the app
        void readImgFile(); // Function to read an image file
        void saveImgFile(); // Function to save an image to an output file
        void convertToGrayScale(); // Function to convert an image to its gray-scale format
        Image getImg() { return img; }
        void setImg(Image _img) { this->img = _img; }
        Menu getMenu() { return menu; }
};

int main() {
    Editor imgEditor;
    imgEditor.run();      
}

// Function declarations of Menu
void Menu::showMainMenu() {
    cout << "MAIN MENU" << endl;
    cout << "0 - Exit" << endl <<
            "1 - Open An Image(D)" << endl <<
            "2 - Save Image Data(D)" << endl <<
            "3 - Scripts(D)" << endl;
}

void Menu::showSubmenu(int opt) { // Takes an option and shows the submenu of that option
    if (opt == 1) {
        cout << "OPEN AN IMAGE MENU" << endl <<
                "0 - UP" << endl <<
                "1 - Enter The Name Of The Image File" << endl;
    } else if (opt == 2) {
        cout << "SAVE IMAGE DATA MENU" << endl <<
                "0 - UP" << endl <<
                "1 - Enter A File Name" << endl;
    } else if (opt == 3) {
        cout << "SCRIPTS MENU" << endl <<
                "0 - UP" << endl <<
                "1 - Convert To Grayscale(D)" << endl;
    } else if (opt == 4) {
        cout << "CONVERT TO GRAYSCALE MENU" << endl <<
                "0 - UP" << endl <<
                "1 - Enter Coefficients For RED GREEN And Blue Channels" << endl;
    }
} 

int Menu::makeChoice(bool isSubmenu) { // A flag (isSubmenu) will be given to avoid code-reuse
    string inp;
    cin >> inp; // Taking user input for the menu
    int choice = this->checkChoice(inp, isSubmenu); // Validating
    return choice;
}

int Menu::checkChoice(string _choice, bool isSubmenu) {
    long inp = this->toInt(_choice); // Coverting input -given as string- to its integer format
    if ((!isSubmenu && inp >= 0 && inp <= 3) || // If the input given from the main menu, then it can be in range of [0,3]
        (isSubmenu && inp >= 0 && inp <= 1)) // If the input given from a submenu, then it can be in range of [0,1]
        return inp;
    return -1; // If the input is invalid, -1 returned; meaning an error has occurred
}

long Menu::toInt(string str) {
    int num = 0;
    for (int i = 0; i < str.length(); i++)
        num += ((str[str.length()-i-1]-48)*pow(10,i)); // Turning each string character to its integer form
    return num; // returning whole number as integer
}

// Function declarations of Image
void Image::setPixels(int val) {
    if (val > MAX_VALUE)
        val = MAX_VALUE;
    this->pixels.push_back(val); // Setting a value of the image
}

void Image::setPixels(float val, int ind) {
    if (val > MAX_VALUE)
        val = MAX_VALUE;
    this->pixels[ind] = val; // Overwriting a spesific value of the image
}

// Function declarations of Editor
void Editor::run() { // This function will run the whole program
    while (true) {
        this->getMenu().showMainMenu();
        int choice = this->getMenu().makeChoice(false); // 'isSubmenu' is false for the request from the main menu
        if (choice <= 0) // Exit if the choice either 0 (exit) or -1 (error)
            return;
        while (true) {
            this->getMenu().showSubmenu(choice);
            int subChoice = this->getMenu().makeChoice(true); // 'isSubmenu' is true since the request is from a submenu
            if (subChoice < 0) // Exit if the choice -1 (error)
                return;
            if (subChoice == 0) { // Go to the upper menu if the choice 0 (UP)
                break;
            } else {
                if (choice == 1) {
                    this->readImgFile();
                } else if (choice == 2) {
                    this->saveImgFile();
                } else if (choice == 3) {
                    while (true) {
                        this->getMenu().showSubmenu(4); // "4" means show the dialog of "Convert To Gray Scale" menu
                        int sSubChoice = this->getMenu().makeChoice(true); // 'isSubmenu' is true since the request is from a submenu
                        if (sSubChoice < 0) // Exit if the choice -1 (error)
                            return;
                        if (sSubChoice == 0) // Go to the upper menu if the choice 0 (UP)
                            break;
                        else
                            this->convertToGrayScale();
                    }
                }
            }
        }
    }
}

void Editor::readImgFile() {
    ifstream inpStream;
    string fileName, str;
    vector<string> data; // Holds data of the given image file 
    cin >> fileName;
    inpStream.open(fileName); // Opening the wanted file
    while (inpStream >> str) // Reading its data
        data.push_back(str);
    // Setting the read row & column values
    this->img.setRow(this->getMenu().toInt(data.at(1)));
    this->img.setColumn(this->getMenu().toInt(data.at(2)));
    // This loop will push the every rgb value to the vector in the Image class
    for (int i = 0; i < this->img.getRow(); i++)
        for (int j = 0; j < this->img.getColumn()*3; j++)
            this->img.setPixels(this->getMenu().toInt(data.at(i*12+j+4))); // Adding 4 to avoid first three lines of the image file
    inpStream.close();
}

void Editor::saveImgFile() {
    string outFile;
    cin >> outFile;
    ofstream outStream(outFile); // Opening an output file to write the new values
    // Info lines must be added to make the file a valid ppm file
    outStream << "P3" << endl << this->getImg().getRow() << " " << this->getImg().getColumn() << endl << MAX_VALUE << endl;
    // This loop will write every rgb value in vector to the output file in a proper/readable style
    for (int i = 0; i < this->img.getRow(); i++) {
        for (int j = 0; j < this->img.getColumn()*3; j+=3) {
            for (int k = j; k < j+3; k++)
                outStream << this->img.getPixels()[i*12+k] << " ";
            outStream << "\t";
        }
        outStream << "\n";
    }
    outStream.close();
}

void Editor::convertToGrayScale() {
    float coeffs[3]; // Holds given coefficients
    while (true) {
        string  inps[3] = {"null", "null", "null"},
                first[3], second[3]; // These hold parts of the input for easy validation
        bool flag = false; // Flag will be true if a given coefficient is invalid
        cin >> inps[0] >> inps[1] >> inps[2];
        for (int i = 0; i < 3; i++) {
            if (inps[i].compare("null") == 0) { // If any coefficient is "null" -meaning no value given-, flag becomes true
                flag = true;
                break;
            } else {
                if (inps[i].compare("0") != 0) { // If the value is different than 0, validation continues
                    int ind = inps[i].find('.'); // Looking for a dot in the given value since it's supposed to be a double
                    if (ind < 0 || ind >= inps[i].length()) { 
                        flag = true; // If index of dot is not in range of the value's size -meaning no dot found-, flag becomes true
                        break;
                    } else {
                        first[i] = inps[i].substr(0, ind); // If a dot found, than the first part is in range of [0, index of dot]
                        if (this->menu.toInt(first[i]) < 0 || this->menu.toInt(first[i]) >= 1) { 
                            flag = true; // If the number is not in range of [0,1], flag becomes true
                            break;
                        } else {
                            second[i] = inps[i].substr(ind+1, inps[i].length()); // Second part in range of [index of dot, length)
                            for (auto c : second[i]) { // Each digit will be checked
                                if ((int)c - 48 < 0 || (int)c - 48 > 9) { // If any of digits is not a number, flag will be true
                                    flag = true;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }                
        if (!flag) { // Meaning all three values are valid coefficients
            // This loop will basically convert the coefficients given as strings to their double forms
            for (int i = 0; i < 3; i++) // If "0.81" is given --> second part/10^^(length of second) --> 81/10^^2 = 0.81
                coeffs[i] = (double)this->menu.toInt(second[i])/pow(10, second[i].length());
            break;
        }
    }
    for (int i = 0; i < this->img.getPixels().size(); i+=3) { // Calculating new values according to the formula:
        float val = (                                   
            this->img.getPixels().at(i) * coeffs[0] +   // RED = (c_r * RED) + (c_g * GREEN) + (c_b * BLUE)
            this->img.getPixels().at(i+1) * coeffs[1] + // GREEN = (c_r * RED) + (c_g * GREEN) + (c_b * BLUE)
            this->img.getPixels().at(i+2) * coeffs[2]   // BLUE = (c_r * RED) + (c_g * GREEN) + (c_b * BLUE)
        );    
        for (int j = i; j < i+3; j++) // Setting new values over old values
            this->img.setPixels(val, j);
    }
}