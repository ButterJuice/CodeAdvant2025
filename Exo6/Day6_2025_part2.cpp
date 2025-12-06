#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define RESET   "\033[0m"

struct Operation {
    std::vector<std::string> values;
    char op;
};

long long operate(long long a, long long b, char op) {
    switch (op) {
        case '+': return a + b;
        case '*': return a * b;
        default: return 0;
    }
}

int main() {
    //std::ifstream file("input");
    std::ifstream file("inputEASY");
    if (!file) {
        std::cerr << "Cannot open input\n";
        return 1;
    }

    std::vector<std::vector<std::string>> rows;
    std::vector<char> operators;
    std::string line;

    // Read all numeric rows first
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        if (line.find_first_of("*+") != std::string::npos) {
            std::stringstream ss(line);
            char c;
            while (ss >> c) {
                operators.push_back(c);
            }
            break;
        }

        std::stringstream ss(line);
        std::vector<std::string> row;
        std::string x;
        while (ss >> x) {
            row.push_back(x);
        }
        rows.push_back(row);
    }

    if (rows.empty() || operators.empty()) {
        std::cerr << "Invalid input format\n";
        return 1;
    }

    size_t cols = operators.size();
    size_t rows_count = rows.size();

    // Build the problems right-to-left, column by column
    std::vector<Operation> operations(cols);
    for (size_t c = 0; c < cols; ++c) {
        operations[c].op = operators[c];
        std::vector<std::string> digits(rows_count);
        for (size_t r = 0; r < rows_count; ++r) {
            // Pad with leading spaces if needed
            digits[r] = (c < rows[r].size()) ? rows[r][c] : "";
        }
        operations[c].values = digits;
    }

    // Now, for each row, assemble the numbers for each problem (column) right-to-left
    std::vector<Operation> ceph_operations;
    for (size_t r = 0; r < rows_count; ++r) {
        Operation op;
        op.op = operators[r];
        for (size_t c = 0; c < cols; ++c) {
            std::string num;
            // For each digit position, take the digit from the corresponding row, right-to-left
            for (size_t d = 0; d < rows_count; ++d) {
                if (c < rows[d].size() && rows[d][c].length() > r) {
                    num += rows[d][c][rows[d][c].length() - 1 - r];
                }
            }
            if (!num.empty())
                op.values.push_back(num);
        }
        ceph_operations.push_back(op);
    }

    // But the correct way is: for each column, assemble the number by reading digits from bottom to top (right-to-left in columns)
    std::vector<Operation> final_operations;
    for (size_t c = 0; c < cols; ++c) {
        Operation op;
        op.op = operators[c];
        for (size_t r = 0; r < rows_count; ++r) {
            if (c < rows[r].size())
                op.values.push_back(rows[r][c]);
        }
        // Reverse to read right-to-left
        std::reverse(op.values.begin(), op.values.end());
        final_operations.push_back(op);
    }

    // For each operation, assemble the number by reading digits column by column (top to bottom), handling blanks
    std::vector<long long> results;
    for (size_t c = 0; c < cols; ++c) {
        std::vector<long long> values;
        size_t max_digits = 0;
        for (size_t r = 0; r < rows_count; ++r)
            max_digits = std::max(max_digits, rows[r][c].size());

        // For each row, build a number by taking the r-th digit from each row (top to bottom)
        for (size_t r = 0; r < rows_count; ++r) {
            std::string num;
            for (size_t d = 0; d < rows_count; ++d) {
                if (c < rows[d].size() && rows[d][c].size() > r) {
                    char ch = rows[d][c][r];
                    if (ch != ' ') num += ch; // skip blanks
                }
            }
            if (!num.empty())
                values.push_back(std::stoll(num));
        }

        long long result = (operators[c] == '*') ? 1 : 0;
        for (auto v : values)
            result = operate(result, v, operators[c]);

        results.push_back(result);

        std::cout << BLUE << "Operation " << c << ": " << RESET;
        for (auto v : values)
            std::cout << YELLOW << v << " " << RESET;
        std::cout << "[" << GREEN << operators[c] << RESET << "] = "
                  << GREEN << result << RESET << "\n";
    }

    long long total = 0;
    for (auto r : results)
        total += r;
    std::cout << YELLOW << "Grand Total: " << total << RESET << "\n";
    return 0;
}
