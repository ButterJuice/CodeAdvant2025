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

void printGrid(const Grid& grid) {
    for (const auto& row : grid.lines) {
        std::cout << row << "\n";
    }
}

//explore the number of paths possible from top to bottom
 int exploreGrid(int counter, Grid& grid, int x, int y) {

    if(y == grid.height)
    {
        // std::cout << "Found path:\n";
        // printGrid(grid);
        // std::cout << "\n";
        return 1;
    }
    if (x < 0 || x >= grid.width || y < 0 )
    {
        return 0;
    }


    if(grid.lines[y][x] == '^')
    {
        return exploreGrid(counter, grid, x - 1, y + 1) + exploreGrid(counter, grid, x + 1, y + 1);
    }else if (grid.lines[y][x] == '.')
    {
       grid.lines[y][x] = '|';
    }
    

    return exploreGrid(counter, grid, x, y + 1);
    

 }


int main() {
    //Grid grid = readGrid("inputMegaEASY");
    Grid grid = readGrid("input");
    // Grid grid = readGrid("inputEASY");

    std::cout << "Initial grid:\n";
    printGrid(grid);
    int total_splits = 0;

    int total_paths = exploreGrid(0, grid, grid.width / 2, 0);


    std::cout << "\nTotal number of splits: " << total_splits << "\n";
    std::cout << "Total number of paths: " << total_paths << "\n";
    return 0;
}
