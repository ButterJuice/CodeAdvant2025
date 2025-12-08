#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#endif

#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"
#define GREEN   "\033[32m"
#define RESET   "\033[0m"

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
void clearScreen() {
    std::cout << "\033[2J\033[H";
}
void printSolutionGrid(const Grid& grid) {
    clearScreen();
    int window_height = std::min(grid.height, 40); // Show up to 40 lines
    for (int i = 0; i < window_height; ++i) {
        for (char c : grid.lines[i]) {
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
    std::cout << "\n" << std::flush;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

long long exploreGrid(Grid& grid, int x, int y) {
    if (x < 0 || x >= grid.width || y < 0 || y >= grid.height)
        return 0;
    char prev = grid.lines[y][x];
    if (prev == '.') grid.lines[y][x] = '|';

    long long result = 0;
    if (y == grid.height - 1) {
        printSolutionGrid(grid);
        result = 1;
    } else if (prev == '^') {
        result = exploreGrid(grid, x - 1, y + 1) + exploreGrid(grid, x + 1, y + 1);
    } else {
        result = exploreGrid(grid, x, y + 1);
    }

    if (prev == '.') grid.lines[y][x] = prev; // backtrack
    return result;
}

int main() {
    enableVirtualTerminal();

    Grid grid = readGrid("inputMedium");

    std::cout << "All solutions:\n\n";
    long long total_paths = exploreGrid(grid, grid.width / 2, 0);

    std::cout << "\nTotal number of paths: " << total_paths << "\n";
    return 0;
}