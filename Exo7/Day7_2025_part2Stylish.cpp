#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <tuple>
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

const int WINDOW_HEIGHT = 40; // Number of lines to show at once

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

void setConsoleSize(int columns, int rows) {
#ifdef _WIN32
    std::string cmd = "mode con: cols=" + std::to_string(columns) + " lines=" + std::to_string(rows);
    system(cmd.c_str());
#endif
}

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

// Memoization cache: key is (x, y)
std::map<std::pair<int, int>, long long> memo;

long long exploreGrid(Grid& grid, int x, int y, int& lastRedLine) {
    if (x < 0 || x >= grid.width || y < 0 || y >= grid.height)
        return 0;
    if (y == grid.height - 1) {
        // Only mark the path if not a split
        if (grid.lines[y][x] == '.') grid.lines[y][x] = '|';
        int centerLine = y;
        printWindow(grid, centerLine);
        updateLineColorWindow(grid.lines[y], y, RED, centerLine, grid.height);
        if (lastRedLine != -1 && lastRedLine != y)
            updateLineColorWindow(grid.lines[lastRedLine], lastRedLine, YELLOW, centerLine, grid.height);
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
        if (grid.lines[y][x] == '|') grid.lines[y][x] = '.'; // backtrack
        lastRedLine = y;
        return 1;
    }

    char prev = grid.lines[y][x];
    // Only mark the path if not a split
    if (prev == '.') grid.lines[y][x] = '|';
    int centerLine = y;
    printWindow(grid, centerLine);
    updateLineColorWindow(grid.lines[y], y, RED, centerLine, grid.height);
    if (lastRedLine != -1 && lastRedLine != y)
        updateLineColorWindow(grid.lines[lastRedLine], lastRedLine, YELLOW, centerLine, grid.height);
    lastRedLine = y;

    long long result = 0;
    if (prev == '^') {
        // Don't mark the current cell, just recurse to the splits
        if (prev == '.') grid.lines[y][x] = '.'; // restore if we marked it
        result = exploreGrid(grid, x - 1, y + 1, lastRedLine) + exploreGrid(grid, x + 1, y + 1, lastRedLine);
    } else {
        result = exploreGrid(grid, x, y + 1, lastRedLine);
    }

    if (prev == '.') grid.lines[y][x] = prev; // backtrack
    return result;
}

int main() {
    enableVirtualTerminal();

    // Grid grid = readGrid("input");
     Grid grid = readGrid("inputMedium");
    // Grid grid = readGrid("inputEASY");
    setConsoleSize(grid.width + 5, WINDOW_HEIGHT + 5);

    printWindow(grid, 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    int lastRedLine = -1;
    long long total_paths = exploreGrid(grid, grid.width / 2, 0, lastRedLine);

    printWindow(grid, grid.height / 2);
    std::cout << "\nTotal number of paths: " << total_paths << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    return 0;
}