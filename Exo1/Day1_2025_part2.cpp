#include <iostream>
#include <fstream>
#include <queue>
#include <string>

//output beautification
#include <iostream>
#include <iomanip> 


int main() {
    
    //std::ifstream infile("inputEASY");
    std::ifstream infile("input");
    if (!infile.is_open()) {
        std::cerr << "Cannot open file\n";
        return 1;
    }

    std::queue<std::string> q;
    std::string line;

    while (std::getline(infile, line)) {
        if (!line.empty()) q.push(line);
    }

    int value = 50;
    int zero_Count = 0;

    std::cout << "Start value: " << value << "\n\n";

    while (!q.empty()) {
        std::string cmd = q.front();
        q.pop();

        char op = cmd[0];
        int num = std::stoi(cmd.substr(1));

        int newValue = value;
        int passes = 0;


        if (op == 'L') {

            if (value == 0) {
                passes = num / 100;
            } 
            else if (num >= value) {
                passes = (num - value) / 100 + 1;   // how many times we cross 0 → 99
            } 
            else {
                passes = 0;
            }

            int nv = (value - num) % 100;
            if (nv < 0) nv += 100;    // normalize
            newValue = nv;

        } 
        else if (op == 'R') {
            long long tot = (long long)value + num;

            passes = tot / 100;       // how many times we cross 99 → 0
            newValue = tot % 100;     // final position

        }
        zero_Count += passes;


        value = newValue;

        // ANSI escape codes:
        // cmd = green, value = blue, passes = red, total count = yellow
        // Set width to 4 characters, right-aligned
        std::cout << "Command: " 
                << "\033[32m" << std::setw(4) << std::right << cmd << "\033[0m"
                << " || Value = " << "\033[34m" << value << "\033[0m";


        if (passes > 0) {
            std::cout << ", Passed " << "\033[31m" << passes << "\033[0m"
                    << " time(s), Total count: " << "\033[33m" << zero_Count << "\033[0m";
        }

        std::cout << "\n";

    }
    std::cout  << " time(s), Total count: " << "\033[33m" << zero_Count << "\033[0m";
        

}
