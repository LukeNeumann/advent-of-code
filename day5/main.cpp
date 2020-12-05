#include <iostream>
#include <vector>
#include <fstream>
#include <bitset>

using std::stoi;

struct seat {
    static constexpr uint n_rows = 128;
    static constexpr uint n_cols = 8;

    std::bitset<n_rows> row = 0;
    std::bitset<n_cols> col = 0;

    seat(std::string s) {
        for(auto& c : s) {
            if(c == 'F') row = (row << 1) | std::bitset<n_rows>(0);
            if(c == 'B') row = (row << 1) | std::bitset<n_rows>(1);
            if(c == 'L') col = (col << 1) | std::bitset<n_cols>(0);
            if(c == 'R') col = (col << 1) | std::bitset<n_cols>(1);
        }
        //std::cout << s << " " << row.to_ulong() << ", " << col.to_ulong() << " " << id() << std::endl;
    }

    constexpr uint id() const {
        return row.to_ulong() * n_cols + col.to_ulong(); 
    }
    static constexpr uint max_id() {
        return n_rows * n_cols; 
    }
};

int main(int argc, char** argv) {
    std::vector<std::string> lines;

    std::ifstream input_file{std::string(argv[1])};
    if(input_file.is_open()) {
        std::string line;
        while(getline(input_file,line)) {
            lines.push_back(line);
        }
    } else {
        std::cout << "Failed to open input file" << std::endl;
    }

    std::vector<bool> seat_map = std::vector<bool>(seat::max_id(), true);

    uint max = 0;
    for(auto& line : lines) {
        seat s = seat(line);
        max = std::max(max, s.id());
        seat_map[s.id()] = false;
    }

    std::cout << max << std::endl;

    for(uint i = 0; i < seat_map.size(); ++i) {
        if(i != 0 && i != seat_map.size() - 1 &&
           !seat_map[i-1] && !seat_map[i+1] && seat_map[i]) {
            std::cout << i << std::endl;
        }
    }


    return 0;
}
