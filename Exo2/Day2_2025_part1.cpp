#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

bool isInvalidID(long long x) {
    std::string s = std::to_string(x);
    int n = s.size();

    if (n % 2 == 1) return false;

    std::string first = s.substr(0, n / 2);
    std::string second = s.substr(n / 2);

    return first == second;
}

// ANSI color escape codes
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define RESET   "\033[0m"

int main() {
    std::ifstream file("input");
    //std::ifstream file("inputEASY");
    if (!file) {
        std::cerr << "Cannot open file\n";
        return 1;
    }

    std::vector<long long> starts;
    std::vector<long long> ends;

    std::string content;
    std::getline(file, content);

    std::stringstream ss(content);
    std::string range;

    while (std::getline(ss, range, ',')) {
        if (range.empty()) continue;

        size_t dash = range.find('-');
        if (dash == std::string::npos) {
            std::cerr << "Invalid range: " << range << "\n";
            continue;
        }

        long long start = std::stoll(range.substr(0, dash));
        long long end   = std::stoll(range.substr(dash + 1));

        starts.push_back(start);
        ends.push_back(end);
    }

    long long total = 0;

    for (size_t i = 0; i < starts.size(); ++i) {

        std::cout << YELLOW
                  << "Checking range: " << starts[i]
                  << "-" << ends[i] << RESET << "\n";

        for (long long id = starts[i]; id <= ends[i]; ++id) {

            if (isInvalidID(id)) {
                std::cout << RED << "INVALID: " << id << RESET << "\n";
                total += id;
            } else {
                std::cout << GREEN << "valid:   " << id << RESET << "\n";
            }
        }
    }

    std::cout << "\nTotal invalid sum = " << total << "\n";

    return 0;
}
