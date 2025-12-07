#include <iostream>
#include <fstream>
#include <vector>
#include <string>

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

int propagateBars(Grid& grid) {
    std::vector<std::string> next = grid.lines;
    int number_of_split = 0;

    for (int y = 1; y < grid.height; ++y) {

        for (int x = 0; x < grid.width; ++x) {

            if (grid.lines[y][x] != '.') continue;

            bool becomes_bar = false;

            // Directly above
            char above = grid.lines[y - 1][x];
            if (above == 'S' || above == '|') {
                becomes_bar = true;
            }

            // left
            if (x > 0 && grid.lines[y][x - 1] == '^' && grid.lines[y-1][x-1] == '|') {
                becomes_bar = true;
                //Only count a split from the left side to avoid double counting
                number_of_split++;
            }

            // right
            if (x < grid.width - 1 && grid.lines[y][x + 1] == '^' && grid.lines[y-1][x+1] == '|') {
                becomes_bar = true;
            }

            if (becomes_bar) {
                next[y][x] = '|';
            }

            
        }
    }

    grid.lines = next;
    return number_of_split;
}


void printGrid(const Grid& grid) {
    for (const auto& row : grid.lines) {
        std::cout << row << "\n";
    }
}

int main() {
    //Grid grid = readGrid("inputMegaEASY");
    Grid grid = readGrid("input");
    //Grid grid = readGrid("inputEASY");

    std::cout << "Initial grid:\n";
    printGrid(grid);
    int total_splits = 0;

    for (int step = 0; step < grid.height; ++step) {
        total_splits += propagateBars(grid);
    }

    std::cout << "\nAfter propagation:\n";
    printGrid(grid);
    std::cout << "\nTotal number of splits: " << total_splits << "\n";
    return 0;
}
