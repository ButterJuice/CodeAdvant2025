#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define RESET   "\033[0m"

void print_entry_info(const std::string& s, const std::vector<size_t>& chosen_indices, const std::string& final_number) {
    std::cout << BLUE << "Entry: " << s << RESET << "\n";

    // Highlight chosen digits
    std::cout << "Highlighted entry: ";
    for (size_t i = 0; i < s.size(); ++i) {
        bool chosen = false;
        for (size_t idx : chosen_indices) {
            if (i == idx) {
                chosen = true;
                break;
            }
        }
        if (chosen)
            std::cout << YELLOW << s[i] << RESET << ' ';
        else
            std::cout << s[i] << ' ';
    }
    std::cout << "\n";

    std::cout << GREEN << "Chosen digits: ";
    for (char c : final_number) std::cout << c;
    std::cout << RESET << "\n";

    std::cout << "Final number: " << final_number << "\n\n";
}

int main() {
    auto start = std::chrono::high_resolution_clock::now(); // Start timer

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

    const int N = 12; // number of digits to choose

    for (const auto& s : entries) {
        std::vector<size_t> chosen_indices;
        std::string final_number;

        size_t L = s.size();
        if (L < N) {
            std::cerr << RED << "Entry too short to pick " << N << " digits!" << RESET << "\n";
            continue;
        }

        size_t start_pos = 0;

        // Pick N digits iteratively
        for (int k = 0; k < N; ++k) {
            int max_digit = -1;
            size_t max_pos = start_pos;

            // We only need to scan up to L - (N - k) to ensure enough digits left
            for (size_t i = start_pos; i <= L - (N - k); ++i) {
                int digit = s[i] - '0';
                if (digit > max_digit) {
                    max_digit = digit;
                    max_pos = i;
                }
            }

            final_number += std::to_string(max_digit);
            chosen_indices.push_back(max_pos);
            start_pos = max_pos + 1; // next pick starts after this digit
        }

        print_entry_info(s, chosen_indices, final_number);

        // Add to total safely if number fits in long long
        total += std::stoll(final_number);
    }

    std::cout << YELLOW << "Total sum: " << total << RESET << "\n";

    auto end = std::chrono::high_resolution_clock::now(); // End timer
    std::chrono::duration<double> elapsed = end - start;
    std::cout << GREEN << "Elapsed time: " << elapsed.count() << " seconds" << RESET << "\n";

    return 0;
}
