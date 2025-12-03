#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define RESET   "\033[0m"

int main() {
    //std::ifstream file("inputEASY");
    std::ifstream file("input");
    if (!file) {
        std::cerr << "Cannot open input\n";
        return 1;
    }

    std::vector<std::string> entries;
    std::string line;

    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        if (!line.empty())
            entries.push_back(line);
    }

    long long total = 0;

    for (const auto& s : entries) {
        std::cout << BLUE << "Entry: " << s << RESET << "\n";



        int best_first = -1, best_second = -1;
        size_t first_idx = 0, second_idx = 0;
        long long best_value = -1;

        // Scan each digit as first
        for (size_t i = 0; i < s.size(); ++i) {
            int first = s[i] - '0';
            int second = -1;
            size_t second_pos = 0;

            // Find the largest digit to the right
            for (size_t j = i + 1; j < s.size(); ++j) {
                int candidate = s[j] - '0';
                if (candidate > second) {
                    second = candidate;
                    second_pos = j;
                }
            }

            if (second == -1) continue; // no digit to the right

            long long value = first * 10 + second;
            if (value > best_value) {
                best_value = value;
                best_first = first;
                best_second = second;
                first_idx = i;
                second_idx = second_pos;
            }
        }

        total += best_value;

        // Print digits with chosen ones highlighted
        std::cout << "Highlighted entry: ";
        for (size_t i = 0; i < s.size(); ++i) {
            if (i == first_idx || i == second_idx)
                std::cout << YELLOW << s[i] << RESET << ' ';
            else
                std::cout << s[i] << ' ';
        }
        std::cout << "\n";

        std::cout << GREEN << "Chosen digits: " << best_first << " and " << best_second << RESET << "\n";
        std::cout << "Final number: " << best_value << "\n\n";
    }

    std::cout << YELLOW << "Total: " << total << RESET << "\n";

    return 0;
}
