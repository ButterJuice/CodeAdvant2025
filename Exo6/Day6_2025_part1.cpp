#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>


#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define WHITE   "\033[37m"
#define RESET   "\033[0m"
struct Operation {
    std::vector<long long> values;
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
    std::ifstream file("input");
    //std::ifstream file("inputEASY");
    if (!file) {
        std::cerr << "Cannot open input\n";
        return 1;
    }

    std::vector<std::vector<long long>> rows;
    std::vector<char> operators;

    std::string line;

    // Read all numeric rows first
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        if (line.find_first_of("*+") != std::string::npos) {
            // Operator line
            std::stringstream ss(line);
            char c;
            while (ss >> c) {
                operators.push_back(c);
            }
            break;
        }

        std::stringstream ss(line);
        std::vector<long long> row;
        long long x;
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
    std::vector<Operation> operations(cols);

    // Initialize operators
    for (size_t c = 0; c < cols; ++c) {
        operations[c].op = operators[c];
    }

    // Fill operations column-wise
    for (const auto& row : rows) {
        for (size_t c = 0; c < cols && c < row.size(); ++c) {
            operations[c].values.push_back(row[c]);
        }
    }
    long long total = 0;
    // Debug output
    for (size_t i = 0; i < operations.size(); ++i) {
        std::cout << BLUE << "Operation " << i << ": " << RESET;
        for (long long v : operations[i].values)
            std::cout << RED << v << " " << RESET;
        std::cout << "[" << GREEN << operations[i].op << RESET << "]\n";

        auto treatedOperation = operations[i];
        long long result;
        if(treatedOperation.op == '*') {
            result = 1;
        } else {
            result = 0;
        }

        for(size_t j = 0; j < treatedOperation.values.size(); ++j) {
            result = operate(result, treatedOperation.values[j], treatedOperation.op);
        }
        std::cout << GREEN << "Result: " << result << RESET << "\n";
        total += result;
    }
    std::cout << YELLOW << "Total: " << total << RESET << "\n";


    
    return 0;
}
