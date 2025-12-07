#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

struct Problem {
    std::vector<long long> numbers;
    char op = '*';
};

int main() {
    std::ifstream file("input");
    //std::ifstream file("inputEASY");
    if (!file) {
        std::cerr << "Cannot open input\n";
        return 1;
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        lines.push_back(line);
    }

    if (lines.size() < 2) {
        std::cerr << "Not enough lines in input\n";
        return 1;
    }

    std::string op_line = lines.back(); // operator row
    lines.pop_back();

    // pad lines
    size_t max_len = 0;
    for (auto &l : lines) max_len = std::max(max_len, l.size());
    for (auto &l : lines) while (l.size() < max_len) l = " " + l;
    while (op_line.size() < max_len) op_line = " " + op_line;

    std::vector<Problem> problems;
    Problem current;
    std::vector<char> problem_ops;

    for (int col = (int)max_len - 1; col >= 0; --col) {
        std::string num_str;
        for (auto &l : lines) {
            if (l[col] != ' ') num_str += l[col];
        }

        if (!num_str.empty()) {
            current.numbers.push_back(std::stoll(num_str));
        }

        // detect empty column separator
        bool next_empty = (col == 0);
        if (!next_empty && col > 0) {
            next_empty = true;
            for (auto &l : lines) {
                if (l[col - 1] != ' ') {
                    next_empty = false;
                    break;
                }
            }
        }

        if (!current.numbers.empty() && (next_empty || col == 0)) {
            // Store the operator for this problem
            char op = '*';
            for (int c = col; c < (int)max_len; ++c) {
                if (op_line[c] == '+' || op_line[c] == '*') {
                    op = op_line[c];
                    break;
                }
            }
            current.op = op;
            problems.push_back(current);
            current = Problem();
        }
    }

    long long grand_total = 0;
    std::cout << "Computation:\n";

    for (auto &p : problems) {
        long long group_result = (p.op == '*') ? 1 : 0;
        for (auto n : p.numbers) {
            if (p.op == '*') group_result *= n;
            else if (p.op == '+') group_result += n;
        }

        std::cout << "Group: ";
        for (size_t i = 0; i < p.numbers.size(); ++i) {
            std::cout << p.numbers[i];
            if (i != p.numbers.size() - 1) std::cout << " " << p.op << " ";
        }
        std::cout << " -> " << group_result << " (Operator: " << p.op << ")\n";

        grand_total += group_result;
    }

    std::cout << "Grand total = " << grand_total << "\n";

    return 0;
}
