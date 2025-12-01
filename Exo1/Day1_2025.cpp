#include <iostream>
#include <fstream>
#include <queue>
#include <string>

int main() {
    std::ifstream infile("input");
    if (!infile.is_open()) {
        std::cerr << "Cannot open file\n";
        return 1;
    }

    std::queue<std::string> q;
    std::string line;

    // Load queue
    while (std::getline(infile, line)) {
        if (!line.empty()) q.push(line);
    }

    int value = 50;
    int zero_Count = 0;

    std::cout << "Start value: " << value << "\n\n";

    // Process queue
    while (!q.empty()) {
        std::string cmd = q.front();
        q.pop();

        char op = cmd[0];              // 'L' or 'R'
        int num = std::stoi(cmd.substr(1));  // the number after it

        if (op == 'L') {
            value = (value - num + 100) % 100; 
        } else if (op == 'R') {
            value = (value + num) % 100;      
        }

        std::cout << "Value = " <<   value ;
        if(value == 0){
            zero_Count++;
            std::cout << "  // Value hit 0! Total count: " << zero_Count ;
        }
        std::cout << "\n" ;
    }

    std::cout << "\nFinal value: " << value << "\n";
    std::cout << "Number of 0: " << zero_Count << "\n";
}
