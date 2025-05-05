#include <iostream>
#include <fstream>
#include <vector>
#include <cmath> // Only 'pow()' funct will be used in 'toInt()'
using namespace std;

// defining max & min values for rgb values
const int MAX_VALUE = 255;
const int MIN_VALUE = 0;

// Function to turn a string to an integer
long toInt(const string& str) {
    int num = 0;
    for (int i = 0; i < str.length(); i++)
        num += ((str[str.length()-i-1]-48)*pow(10,i)); // Turning each string character to its integer form
    return num; // returning whole number as integer
}

// Function to check if an input is integer
const bool isValid(const string inp) {
    for (auto val : inp)
        if (val-48 < 0 && val-48 > 9)
            return false;
    return true;
}

class Pixel {
    private:
        int r = MAX_VALUE;
        int g = MAX_VALUE;
        int b = MAX_VALUE;
    public:
        Pixel() { /* Intentionally left empty */ };

        Pixel(const int& _r, const int& _g, const int& _b) { // Creates a pixel with given values
            this->setR(_r);
            this->setG(_g);
            this->setB(_b);
        }

        const void printInfo() const { // Prints info of a pixel
            cout << this->getR() << " " << this->getG() << " " << this->getB() << endl;
        }

        const int& getR() const { return r; }
        const int& getG() const { return g; }
        const int& getB() const { return b; }

        void setR(int _r) {
            if ( _r > MAX_VALUE) 
                _r = MAX_VALUE;
            else if (_r < MIN_VALUE)
                _r = MIN_VALUE;
            this->r = _r;
        }

        void setG(int _g) {
            if ( _g > MAX_VALUE) 
                _g = MAX_VALUE;
            else if (_g < MIN_VALUE)
                _g = MIN_VALUE;
            this->g = _g;
        }

        void setB(int _b) {
            if ( _b > MAX_VALUE) 
                _b = MAX_VALUE;
            else if (_b < MIN_VALUE)
                _b = MIN_VALUE;
            this->b = _b;
        }
};

class ppmImage {
    private:
        vector<Pixel> pixels; // Holds RGB values of image
        string format; // Holds the format of the image, P3 for this project
        int row; // Holds row value of image
        int column; // Holds column value of image
    public:
        ppmImage() { /* Intentionally left empty */ }
        ppmImage(const string& fileName) { this->readImgFile(fileName); }
        ppmImage(const string&, const string&); // Creates a blank image with given dimensions 
        ppmImage(const int&, const int&); // Creates a blank image
        const bool readImgFile(string="null"); // Function to read an image file
        const bool saveImgFile(string="null") const; // Function to save an image to an output file
        void createImg(const vector<string>&); // Creates an image with given data
        
        const void printDimensions() { cout << this->getRow() << " " << this->getColumn() << endl; }
        vector<Pixel>& getPixels() { return pixels; }
        const string& getFormat() const { return format; }
        const int& getRow() const { return row; }
        const int& getColumn() const { return column; }
        void setFormat(const string& _format) { this->format = _format; }
        
        const void printPixInfo(const string& ind) { // Prints info of an individual pixel
            // If given index is an integer and in range of dimensions of the image
            if (isValid(ind) && toInt(ind) < this->getPixels().size())
                this->getPixels()[toInt(ind)].printInfo();
        }   

        void setPixels(const int& r=MAX_VALUE, const int& g=MAX_VALUE, const int& b=MAX_VALUE) { // Pushes a pixel to the vector
            this->pixels.push_back(Pixel(r,g,b));
        }

        void setPixels(const int& r, const int& g, const int& b, const int& ind) { // Overwrites RGB values of a given pixel
            this->pixels[ind].setR(r);
            this->pixels[ind].setG(g);
            this->pixels[ind].setB(b);
        }

        void setRow(int _row) {
            if (_row <= MIN_VALUE)
                _row = MIN_VALUE;
            this->row = _row;
        }

        void setColumn(int _column) {
            if (_column <= MIN_VALUE)
                _column = MIN_VALUE;
            this->column = _column;
        }

        const ppmImage operator+(ppmImage&);
        const ppmImage operator-(ppmImage&);
        friend ostream& operator<<(ostream&, ppmImage&);
        const int& operator()(const int&, const int&, const int&);
};

// Standlone Functions
int read_ppm(const string, ppmImage&);
int write_ppm(const string, const ppmImage&);
int test_addition(const string, const string, const string);
int test_subtraction(const string, const string, const string);
int test_print(const string);
int swap_channels(ppmImage&, int);
ppmImage single_color(const ppmImage&, int);

int main(int argc, char** argv) {
    if (argc < 3 || argc > 5)
        return 1;
    if (!isValid(argv[1]))
        return 1;
    ppmImage img, newImg;
    switch(*argv[1]) {
        case '1':
            if (argc != 5)
                return 1;
            test_addition(argv[2], argv[3], argv[4]);
            break;
        case '2':
            if (argc != 5)
                return 1;
            test_subtraction(argv[2], argv[3], argv[4]);
            break;
        case '3':
            if (argc < 4)
                return 1;
            read_ppm(argv[2], img);
            swap_channels(img, 2);
            write_ppm(argv[3], img);
            break;
        case '4':
            if (argc < 4)
                return 1;
            read_ppm(argv[2], img);
            swap_channels(img, 3);
            write_ppm(argv[3], img);
            break;
        case '5':
            read_ppm(argv[2], img);
            newImg = single_color(img, 1);
            write_ppm(argv[3], newImg);
            break;
        case '6':
            read_ppm(argv[2], img);
            newImg = single_color(img, 2);
            write_ppm(argv[3], newImg);
            break;
        case '7':
            read_ppm(argv[2], img);
            newImg = single_color(img, 3);
            write_ppm(argv[3], newImg);
            break; 
    }
    return 0;
}

// Standalone functions' declarations
int read_ppm(const string source_ppm_file_name, ppmImage& destination_object) {
    if (!destination_object.readImgFile(source_ppm_file_name))
        return 0;
    return 1;
}

int write_ppm(const string destination_ppm_file_name, const ppmImage& source_object) {
    if (!source_object.saveImgFile(destination_ppm_file_name))
        return 0;
    return 1;
}

int test_addition(const string filename_image1, const string filename_image2, const string filename_image3) {
    ppmImage img1, img2;
    if(!img1.readImgFile(filename_image1) || !img2.readImgFile(filename_image2))
        return 0;
    ppmImage sumImg = img1 + img2;
    if (!sumImg.saveImgFile(filename_image3))
        return 0;
    return 1;
}

int test_subtraction(const string filename_image1, const string filename_image2, const string filename_image3) {
    ppmImage img1, img2;
    if(!img1.readImgFile(filename_image1) || !img2.readImgFile(filename_image2))
        return 0;
    ppmImage difImg = img2 - img1;
    if (!difImg.saveImgFile(filename_image3))
        return 0;
    return 1;
}

int test_print(const string filename_image1) {
    ppmImage tempImg;
    if (!tempImg.readImgFile(filename_image1))
        return 0;
    cout << tempImg;
    return 1;
}

int swap_channels(ppmImage& image_object_to_be_modified, int swap_choice) {
    int row = image_object_to_be_modified.getRow(),
        column = image_object_to_be_modified.getColumn();
    if (row == 0 || column == 0)
        return 0;
    for (int i = 0; i < row*column; i++) {
        int red = image_object_to_be_modified.getPixels()[i].getR(),
            green = image_object_to_be_modified.getPixels()[i].getG(),
            blue = image_object_to_be_modified.getPixels()[i].getB();
        if (swap_choice == 1)
            image_object_to_be_modified.setPixels(green, red, blue, i);
        else if (swap_choice == 2)
            image_object_to_be_modified.setPixels(blue, green, red, i);
        else if (swap_choice == 3)
            image_object_to_be_modified.setPixels(red, blue, green, i);
    }
    return 1;    
}

ppmImage single_color(const ppmImage& source, int color_choice) {
    ppmImage newImg(source.getRow(), source.getColumn()),
            sourceImg = source;
    for (int i = 0; i < source.getRow()*source.getColumn(); i++) {
        if (color_choice == 1)
            newImg.setPixels(sourceImg.getPixels()[i].getR(), 0, 0, i);
        else if (color_choice == 2)
            newImg.setPixels(0, sourceImg.getPixels()[i].getG(), 0, i);
        else
            newImg.setPixels(0, 0, sourceImg.getPixels()[i].getB(), i);
    }
    return newImg;
}

// Function declarations of ppmImage
ppmImage::ppmImage(const string& row, const string& column) {    
    isValid(row) ? this->setRow(toInt(row)) : this->setRow(0);
    isValid(column) ? this->setColumn(toInt(column)) : this->setColumn(0);
    // Pushing each RGB value to the vector in the ppmImage class
    for (int i = 0; i < this->getRow()*this->getColumn(); i++)
        this->setPixels(255, 255, 255);
}

ppmImage::ppmImage(const int& row, const int& column) {    
    this->setRow(row);
    this->setColumn(column);
    // Pushing each RGB value to the vector in the ppmImage class
    for (int i = 0; i < this->getRow()*this->getColumn(); i++)
        this->setPixels(255, 255, 255);
}

const bool ppmImage::readImgFile(string fileName) {
    ifstream inpStream;
    string str;
    vector<string> data; // Holds data of the given image file
    if (fileName.compare("null") == 0) 
        cin >> fileName;
    inpStream.open(fileName); // Opening the given file
    if (!inpStream) // If the file does not exist, return failed
        return false;
    while (inpStream >> str) // Reading file's data
        data.push_back(str);
    this->createImg(data);
    inpStream.close();
    return true;
}

const bool ppmImage::saveImgFile(string outFile) const {
    if (outFile.compare("null") == 0)
        cin >> outFile;
    if (this->getRow() == 0 || this->getColumn() == 0)
        return false;
    ofstream outStream(outFile); // Opening an output file to write the new values
    // Info lines must be added to make the file a valid ppm file
    outStream << "P3" << endl << this->getRow() << " " << this->getColumn() << endl << MAX_VALUE << endl;
    // This loop will write each RGB value in the vector to the output file in a proper/readable style
    ppmImage tempImg = *this;
    for (int i = 0; i < this->getRow(); i++) {
        for (int j = 0; j < this->getColumn(); j++) {
            outStream <<
                tempImg.getPixels()[i*this->getColumn()+j].getR() << " " <<
                tempImg.getPixels()[i*this->getColumn()+j].getG() << " " << 
                tempImg.getPixels()[i*this->getColumn()+j].getB() << "\t";
        }
        outStream << "\n";
    }
    outStream.close();
    return true;
}

void ppmImage::createImg(const vector<string>& data) {
    // Setting format & row & column values
    this->setFormat(data.at(0));
    this->setRow(toInt(data.at(1)));
    this->setColumn(toInt(data.at(2)));
    // Pushing each RGB value to the vector in the ppmImage class
    for (int i = 4; i < this->getRow()*this->getColumn()*3+4; i+=3) {
            this->setPixels(
                toInt(data.at(i)),
                toInt(data.at(i+1)),
                toInt(data.at(i+2)));
    }
} 

const ppmImage ppmImage::operator+(ppmImage& img2) {
    if (this->getRow() != img2.getRow() || this->getColumn() != img2.getColumn())
        return ppmImage(0, 0);
    ppmImage newImg(this->getRow(), this->getColumn());
    for (int i = 0; i < this->getRow()*this->getColumn(); i++) {
        newImg.setPixels(
            this->getPixels()[i].getR() + img2.getPixels()[i].getR(),
            this->getPixels()[i].getG() + img2.getPixels()[i].getG(),
            this->getPixels()[i].getB() + img2.getPixels()[i].getB(),
            i
        );
    }
    return newImg;
}

const ppmImage ppmImage::operator-(ppmImage& img2) {
    if (this->getRow() != img2.getRow() || this->getColumn() != img2.getColumn())
        return ppmImage(0, 0);
    ppmImage newImg(this->getRow(), this->getColumn());
    for (int i = 0; i < this->getRow()*this->getColumn(); i++) {
        newImg.setPixels(
            this->getPixels()[i].getR() - img2.getPixels()[i].getR(),
            this->getPixels()[i].getG() - img2.getPixels()[i].getG(),
            this->getPixels()[i].getB() - img2.getPixels()[i].getB(),
            i
        );
    }
    return newImg;
}

ostream& operator<<(ostream& outs, ppmImage& img) {
    cout << img.getFormat() << endl <<
            img.getRow() << " " << img.getColumn() << endl <<
            MAX_VALUE << endl;
    for (int i = 0; i < img.getRow(); i++) {
        for (int j = 0; j < img.getColumn(); j++) {
            outs <<
                img.getPixels()[i*img.getColumn()+j].getR() << " " <<
                img.getPixels()[i*img.getColumn()+j].getG() << " " << 
                img.getPixels()[i*img.getColumn()+j].getB() << "\t";
        }
        outs << "\n";
    }
    return outs;
}

const int& ppmImage::operator()(const int& row, const int& column, const int& clrChn) {
    if (clrChn == 1)
        return this->getPixels()[row*this->getColumn()+column].getR();
    else if (clrChn == 2)
        return this->getPixels()[row*this->getColumn()+column].getG();
    else
        return this->getPixels()[row*this->getColumn()+column].getB();
}