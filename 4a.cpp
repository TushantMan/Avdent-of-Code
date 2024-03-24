#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <regex>
#include <map>
#include <unordered_map>
#include <unordered_set>

using namespace std;

int checkCard(const std::unordered_set<int> &winning_numbers, const std::vector<int> &card_number)
{
    int point = 0;
    for (int number : card_number) {
        if (auto iter = winning_numbers.find(number); iter != winning_numbers.end()) {
            if (point == 0) {
                point = 1;
            } else {
                point *= 2;
            }
        }
    }
    return point;
}

int main()
{
    std::ifstream ifs;
    ifs.open("day4_input.txt");

    std::string line;
    unsigned sum = 0;

    while (std::getline(ifs, line)) {
        std::unordered_set<int> winning_numbers;
        std::vector<int> card_numbers;

        std::size_t found_colon = line.find(':');
        std::size_t found_pipe = line.find('|');

        string winning_str = line.substr(found_colon + 1, found_pipe - found_colon - 1);
        string card_Str = line.substr(found_pipe + 1, line.length() - found_pipe - 1);

        // read winning numbers
        {
            stringstream ss;
            ss << winning_str;
            string temp;
            int found;
            while (!ss.eof()) {
                ss >> temp;
                if (stringstream(temp) >> found) {
                    winning_numbers.insert(found);
                }
                temp = "";
            }
        }
        

        // read card numbers
        {
            stringstream ss;

            ss << card_Str;
            string temp;
            int found;
            while (!ss.eof()) {
                ss >> temp;
                if (stringstream(temp) >> found) {
                    card_numbers.push_back(found);
                }
                temp = "";
            }
        }

        int res = checkCard(winning_numbers, card_numbers);
        sum += res;
    }

    std::cout << "The puzzle answer is: " << sum << endl;
    return 0;
}