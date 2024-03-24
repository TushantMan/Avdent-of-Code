#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

int main(){
    ifstream infile;
    vector<int> numberList; // Vector to store numbers read from the file
    infile.open("day1_input.txt"); 
    string currentLine, currentNum; // Variables to store current line and number
    int digitPos; // Variable to store the position of the digit in the line
    while(!infile.eof()){ 
        getline(infile, currentLine);
        // Find the position of the first digit in the line & append the digit to the current number
        digitPos = currentLine.find_first_of("1234567890"); 
        currentNum += currentLine[digitPos]; 

        digitPos = currentLine.find_last_of("1234567890"); 
        currentNum += currentLine[digitPos]; 
        // Convert the current number to integer and store it in the vector
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
