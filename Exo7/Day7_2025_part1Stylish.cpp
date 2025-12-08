#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#endif



#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define RESET   "\033[0m"


struct Cell {
    int x;
    int y;
    char type;
};

struct Grid {
    int width;
    int height;
    std::vector<std::string> lines;
};

Grid readGrid(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Cannot open input\n";
        std::exit(1);
    }

    Grid grid;
    std::string line;

    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        grid.lines.push_back(line);
    }

    grid.height = grid.lines.size();
    grid.width  = grid.lines[0].size();
    return grid;
}

void enableVirtualTerminal() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;
    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
#endif
}

void printGrid(const Grid& grid) {
    for (const auto& row : grid.lines) {
        for (char c : row) {
            if (c == '|') {
                std::cout << YELLOW << c << RESET;
            } else if (c == '^') {
                std::cout << CYAN << c << RESET;
            } else if (c == 'S') {
                std::cout << GREEN << c << RESET;
            } else {
                std::cout << c;
            }
        }
        std::cout << "\n";
    }
}

void setConsoleSize(int columns, int rows) {
#ifdef _WIN32
    std::string cmd = "mode con: cols=" + std::to_string(columns) + " lines=" + std::to_string(rows);
    system(cmd.c_str());
#endif
}

void updateLineColor(const std::string& row, int y, const char* color) {
    std::cout << "\033[" << (y + 1) << ";1H";
    for (char c : row) {
        if (c == '|') {
            std::cout << color << c << RESET;
        } else if (c == '^') {
            std::cout << CYAN << c << RESET;
        } else if (c == 'S') {
            std::cout << GREEN << c << RESET;
        } else {
            std::cout << c;
        }
    }
    std::cout << "\033[0K";
}

const int WINDOW_HEIGHT = 40; // Number of lines to show at once

void printWindow(const Grid& grid, int centerLine) {
    int start = std::max(0, std::min(centerLine - WINDOW_HEIGHT / 2, grid.height - WINDOW_HEIGHT));
    int end = std::min(grid.height, start + WINDOW_HEIGHT);
    std::cout << "\033[H"; // Move cursor to top left
    for (int y = start; y < end; ++y) {
        for (char c : grid.lines[y]) {
            if (c == '|') {
                std::cout << YELLOW << c << RESET;
            } else if (c == '^') {
                std::cout << CYAN << c << RESET;
            } else if (c == 'S') {
                std::cout << GREEN << c << RESET;
            } else {
                std::cout << c;
            }
        }
        std::cout << "\n";
    }
    // Clear remaining lines in the window if grid is smaller than window
    for (int y = end; y < start + WINDOW_HEIGHT; ++y) {
        std::cout << "\033[0K\n";
    }
}

void updateLineColorWindow(const std::string& row, int y, const char* color, int centerLine, int gridHeight) {
    int start = std::max(0, std::min(centerLine - WINDOW_HEIGHT / 2, gridHeight - WINDOW_HEIGHT));
    int lineInWindow = y - start;
    if (lineInWindow < 0 || lineInWindow >= WINDOW_HEIGHT) return;
    std::cout << "\033[" << (lineInWindow + 1) << ";1H";
    for (char c : row) {
        if (c == '|') {
            std::cout << color << c << RESET;
        } else if (c == '^') {
            std::cout << CYAN << c << RESET;
        } else if (c == 'S') {
            std::cout << GREEN << c << RESET;
        } else {
            std::cout << c;
        }
    }
    std::cout << "\033[0K";
}


int propagateBars(Grid& grid, int& lastRedLine) {
    std::vector<std::string> next = grid.lines;
    int number_of_split = 0;
    int newRedLine = -1;

    for (int y = 1; y < grid.height; ++y) {
        bool changed = false;
        for (int x = 0; x < grid.width; ++x) {
            if (grid.lines[y][x] != '.') continue;

            bool becomes_bar = false;
            char above = grid.lines[y - 1][x];
            if (above == 'S' || above == '|') {
                becomes_bar = true;
            }
            if (x > 0 && grid.lines[y][x - 1] == '^' && grid.lines[y-1][x-1] == '|') {
                becomes_bar = true;
                number_of_split++;
            }
            if (x < grid.width - 1 && grid.lines[y][x + 1] == '^' && grid.lines[y-1][x+1] == '|') {
                becomes_bar = true;
            }
            if (becomes_bar) {
                next[y][x] = '|';
                changed = true;
            }
        }
        if (changed) {
            newRedLine = y;
            break; // Only animate the first changed line per step
        }
    }
    int centerLine = (newRedLine != -1) ? newRedLine : lastRedLine;
    printWindow(grid, centerLine);

    if (lastRedLine != -1) {
        updateLineColorWindow(next[lastRedLine], lastRedLine, YELLOW, centerLine, grid.height);
    }
    if (newRedLine != -1) {
        updateLineColorWindow(next[newRedLine], newRedLine, RED, centerLine, grid.height);
        std::this_thread::sleep_for(std::chrono::milliseconds(80));
    }

    lastRedLine = newRedLine;
    grid.lines = next;
    return number_of_split;
}

int main() {
    enableVirtualTerminal();

    Grid grid = readGrid("input");
    setConsoleSize(grid.width + 5, WINDOW_HEIGHT + 5);

    int total_splits = 0;
    printWindow(grid, 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    int lastRedLine = -1;
    for (int step = 0; step < grid.height; ++step) {
        total_splits += propagateBars(grid, lastRedLine);
    }

    printWindow(grid, grid.height / 2);
    std::cout << "\nTotal number of splits: " << total_splits << "\n";
    
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    return 0;
}