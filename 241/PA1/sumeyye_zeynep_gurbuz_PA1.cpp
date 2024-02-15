#include <iostream>
#include <math.h>
#include <time.h>
#include <cstring>

using namespace std;

string input(int); // Takes a guess from the user
int firstCount(string, int[]);  // Counts the matching digits
int secondCount(string, int[]); // Counts the existing but not matching digits
int createDigit(bool); // Creates a random digit
int validateInput(int, char*[]); // Checks if correct parameters given
bool validateNumber(char[], int); // Checks if the given secret number is valid
long toInt(string); // Turns the given string to integer

int main(int argc, char* argv[]) {
    srand(time(0));

    int size = validateInput(argc, argv); // Validates command line arguments
    if (size == -1) { // -1: wrong/undefined parameters
        cerr << "E0" << endl;
        return 0;
    }
    // Declaring & initializing an int-array to store the secret number
    int num[size];
    for (int i = 0; i < size; i++)
        num[i] = -1;
    
    if (strcmp("-u", argv[1]) == 0) {
        string str = argv[2]; // The given number will be used as the secret number
        for (int i = 0; i < size; i++)
            num[i] = (int)str.at(i) - 48;
    } else { // If the random option is chosen, a random secret number will be created
        bool isUsed[10]; // Declaring & initializing a bool-array to track available digits
        for (int i = 0; i < 10; i++)
            isUsed[i] = false;
        // Each digit will be created individually; if it has already been used once, it will be marked as unavailable
        for (int i = 0; i < size; i++) {
            int tempDigit = -1;
            if (i == 0)
                tempDigit = createDigit(true);
            else
                tempDigit = createDigit(false);
            if (isUsed[tempDigit]) {
                --i; // If a repeating digit is created, the loop will be done again
            } else {
                num[i] = tempDigit;
                isUsed[tempDigit] = true; // Marking the used digit as unavailable
            }
        }
    }

    for (int iter = 1; iter <= 100; iter++) {
        string guess = input(size); // Taking a guess from the user
        if (guess.compare("E1") == 0) { // User entered more or less digits than required
            cerr << "E1" << endl;
            return 0;
        } else if (guess.compare("E2") == 0) { // User entered a non-integer value
            cerr << "E2" << endl;
            return 0;
        } else { // User entered a valid guess
            int corrDigs = firstCount(guess, num);
            int guesDigs = secondCount(guess, num);
            if (corrDigs == size) { // If all digits guessed correct, the program finishs
                cout << "FOUND " << iter << endl;
                return 1;
            }
            cout << corrDigs << " " << guesDigs << endl; // Printing hints for the next iteration
        }
    }
    cout << "FAILED"; // If the secret number couldn't been guessed for 100 times, user fails
    return 0;
}

string input(int size) {
    string guess;
    cin >> guess;
    if (guess.length() != size) // Checking if the length of the guess is valid
        return "E1";
    for (int i = 0; i < guess.length(); i++) // Checking if any non-integer value is given
        if ((int)guess.at(i)-48 < 0 || (int)guess.at(i)-48 > 9)
            return "E2";
    return guess; // Valid guess
}

int firstCount(string guess, int secretNum[]) {
    int correctDigits = 0;
    for (int i = 0; i < guess.length(); i++)
        if ((int)guess[i]-48 == secretNum[i]) // If a digit matches for both numbers, counter increases
            ++correctDigits;
    return correctDigits;
}

int secondCount(string guess, int secretNum[]) {
    int guessedDigits = 0;
    for (int i = 0; i < guess.length(); i++)
        for (int j = 0; j < guess.length(); j++)
            if (secretNum[i] == (int)guess[j]-48 && i != j) // If a digit exists in both numbers but its position is wrong, counter increases 
                ++guessedDigits;
    return guessedDigits;
}

int createDigit(bool isZero) { // If it is the first digit of the number, zero will be excluded
    return (isZero) ? (int)(rand() % 9) + 1 : (int)(rand() % 10);
}

int validateInput(int argc, char* argv[]) {
    if (argc < 3 || argc > 3) // Checking if command line arguments are valid
        return -1;

    if (strcmp("-r", argv[1]) == 0) { // If the random option is chosen
        int num = toInt(argv[2]);
        if (num <= 0 || num > 9) // Checking if the given digit number is valid
            return -1;
        else
            return num; // Valid size
    } else if (strcmp("-u", argv[1]) == 0) { // If the user gave the secret number
        string inp = argv[2];
        if (inp.at(0)-48 == 0) // Checking if the first digit is zero or not
            return -1;
        for (auto ch : inp)
            if ((int)ch-48 < 0 || (int)ch-48 > 9) // Checking if a non-integer value is given
                return -1;
        return validateNumber(argv[2], inp.length()) ? inp.length() : -1; // Checking if the given secret number is valid 
    } else { // If unvalid command line arguments given
        return -1;
    }
}

bool validateNumber(char arr[], int size) {
    bool isUsed[10]; // Declaring & initializing a bool-array to track available digits
    for (int i = 0; i < 10; i++)
        isUsed[i] = false;
    
    for (int i = 0; i < size; i++) {
        if (isUsed[arr[i]-48]) // If the given number has any non-unique digits, return false
            return false;
        isUsed[arr[i]-48] = true; // Marking a used digit as unavaliable
    }
    return true; // Valid secret number
}

long toInt(string str) {
    int num = 0;
    for (int i = 0; i < str.length(); i++)
        num += ((str[str.length()-i-1]-48)*pow(10,i)); // Turning each string character to its integer form
    return num; // returning whole number as integer
}