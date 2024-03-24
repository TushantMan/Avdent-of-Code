#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

// Function to find the first and last occurrence of a word digit in the current line
int findFirstWordDigit(string currentLine, int digitPos, string wordDigits[]);
int findLastWordDigit(string currentLine, int digitPos, string wordDigits[]);

int main(){
    ifstream infile; 
    // Vector to store numbers read from the file
    vector<int> numberList; 
    // Array of word digits
    string wordDigits[] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"}; 

    infile.open("day1_input.txt");
    string currentLine, currentNum;
    int digitPos, wordDigitNum; 
    while(!infile.eof()){ 
        getline(infile, currentLine); 

        digitPos = currentLine.find_first_of("1234567890"); 
        wordDigitNum = findFirstWordDigit(currentLine, digitPos, wordDigits); 
        if(wordDigitNum > 0){ 
            currentNum += to_string(wordDigitNum); 
        }else{
            currentNum += currentLine[digitPos]; 
        }

        digitPos = currentLine.find_last_of("1234567890"); 
        wordDigitNum = findLastWordDigit(currentLine, digitPos, wordDigits); 
        if(wordDigitNum > 0){ 
            currentNum += to_string(wordDigitNum); 
        }else{
            currentNum += currentLine[digitPos]; 
        }

        numberList.push_back(stoi(currentNum));

        currentLine.clear();
        currentNum.clear();
    }
    infile.close();

    int sum = 0; 
    for(int i = 0; i < numberList.size(); i++){
        sum += numberList.at(i); 
    }

    cout << "The puzzle answer is: " << sum << endl; 
}

// Function to find the first occurrence of a word digit in the current line
int findFirstWordDigit(string currentLine, int digitPos, string wordDigits[]){
    int wordDigitPos;
    int index = -1;

    for(int i = 0; i < 9; i++){ 
        wordDigitPos = currentLine.find(wordDigits[i]); 
        if(wordDigitPos < digitPos && wordDigitPos != string::npos){ 
            digitPos = wordDigitPos;
            index = i;
        }
    }
    return index + 1; 
}

// Function to find the last occurrence of a word digit in the current line
int findLastWordDigit(string currentLine, int digitPos, string wordDigits[]){
    int wordDigitPos;
    int index = -1;

    for(int i = 0; i < 9; i++){ 
        wordDigitPos = currentLine.rfind(wordDigits[i]); 
        if(wordDigitPos > digitPos && wordDigitPos != string::npos){ 
            digitPos = wordDigitPos; 
            index = i; 
        }
    }

    return index + 1;
}
