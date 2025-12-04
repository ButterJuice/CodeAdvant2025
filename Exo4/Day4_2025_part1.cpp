#include <iostream>
#include <fstream>
#include <string>
#include <vector>


#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define RESET   "\033[0m"

std::vector<std::string> readGrid(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<std::string> grid;
    std::string line;

    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        if (!line.empty())
            grid.push_back(line);
    }
    return grid;
}


int countAdjacent(const std::vector<std::string>& grid, int r, int c) {
    int H = grid.size();
    int W = grid[0].size();
    int count = 0;

    // all eight directions
    int dr[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dc[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for (int i = 0; i < 8; ++i) {
        int nr = r + dr[i];
        int nc = c + dc[i];

        if (nr >= 0 && nr < H && nc >= 0 && nc < W) {
            if (grid[nr][nc] == '@')
                count++;
        }
    }
    return count;
}


int main() {
    std::vector<std::string> grid = readGrid("input");
    //std::vector<std::string> grid = readGrid("inputEASY");

    int H = grid.size();
    int W = grid[0].size();

    int total_paper = 0;
    int total_accessible = 0;
    int total_non_accessible = 0;
    for (int r = 0; r < H; ++r) {
        for (int c = 0; c < W; ++c) {
            if(grid[r][c] == '@'){
                
                total_paper++;
                int adj = countAdjacent(grid, r, c);
                if(adj >= 4){
                  total_non_accessible++;
                    std::cout  << YELLOW << adj << ' ' << RESET;
                }
                else
                    std::cout << adj << ' ' ;
            }else
                std::cout <<". " ;
                
        }
        std::cout << "\n";
    }
    total_accessible = total_paper - total_non_accessible; 
    std::cout << "Total: " << total_accessible << "\n";
}
