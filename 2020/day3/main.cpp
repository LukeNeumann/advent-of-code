#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

struct tree_grid {
    std::vector<std::vector<bool> > trees;

    void add_row(std::string row) {
        width = std::max(width, row.size());
        std::vector<bool> tree_row(width, false);
        for(uint i = 0; i < width; i++) {
            if(row[i] == '#') tree_row[i] = true;
        }

        trees.push_back(tree_row);
    }

    bool check(uint x, uint y){
        return trees[y][x % width];
    }

    uint count_slope(uint right, uint down) {
        uint x = 0, y = 0;
        uint hit_count = 0;
        while(y < trees.size()) {
            hit_count += check(x, y);
            x += right;
            y += down;
        }

        return hit_count;
    }

    private:
    unsigned long width = 0;
};


int main(int argc, char** argv) {
    tree_grid tg;

    std::ifstream input_file{std::string(argv[1])};
    if(input_file.is_open()) {
        std::string line;
        while(getline(input_file,line)) {
            tg.add_row(line);
        }
    } else {
        std::cout << "Failed to open input file" << std::endl;
    }

    std::cout << tg.count_slope(3, 1) << " trees hit" << std::endl;
    std::cout << "part 2: " << 
        tg.count_slope(1, 1) *
        tg.count_slope(3, 1) *
        tg.count_slope(5, 1) *
        tg.count_slope(7, 1) *
        tg.count_slope(1, 2) 
    << std::endl;

    return 0;
}
