#include <iostream>
#include <vector>
#include <fstream>
#include <numeric>
#include <algorithm>
#include <map>
#include <iterator>
#include <functional>
#include <boost/range/algorithm/count.hpp>

#include "input_handler.hpp"

using std::vector;
using std::string;
using std::map;
using std::pair;
using std::cout;
using std::endl;

struct seat_map {
    seat_map(vector<string> rows) {
        width = rows[0].size();
        height = rows.size();
        for(auto& row : rows) {
            state += row;
        }
    }

    void apply_rules() {
        prev_state = state;
        
        for(size_t i = 0; i < state.size(); i++) {
            int col = i % width;
            int row = i / width;
            uint adjacent_occupied = 0;
            for(int r = std::max(0, row-1); r <= std::min(height-1, row+1); r++) {
                for(int c = std::max(0, col-1); c <= std::min(width-1, col+1); c++) {
                    int index = c + width * r;
                    if(index != i) {
                        if(prev_state[index] == '#') adjacent_occupied++;
                    }
                }
            }

            if(prev_state[i] == 'L' && adjacent_occupied == 0) state[i] = '#'; 
            if(prev_state[i] == '#' && adjacent_occupied >= 4) state[i] = 'L'; 
        }
    }

    void apply_rules2() {
        prev_state = state;
        
        for(size_t i = 0; i < state.size(); i++) {
            if(prev_state[i] == '.') continue;
            
            int col = i % width;
            int row = i / width;
            uint adjacent_occupied = 0;
            for(int dirx = -1; dirx <= 1; dirx++){
                for(int diry = -1; diry <= 1; diry++){
                    if(dirx == 0 && diry == 0) continue;

                    int c = col + dirx, r = row + diry;
                    while(c >= 0 && c < width && r >= 0 && r < height) {
                        int index = c + width * r;
                        if(prev_state[index] == '#') adjacent_occupied++;
                        if(prev_state[index] != '.') {
                            break;
                        }
                        c += dirx;
                        r += diry;
                    }
                }
            }

            if(prev_state[i] == 'L' && adjacent_occupied == 0) state[i] = '#'; 
            if(prev_state[i] == '#' && adjacent_occupied >= 5) state[i] = 'L'; 
        }
    }

    void stabilize(int mode = 1) {
        while(prev_state != state) {
            if(mode == 1) apply_rules();
            if(mode == 2) apply_rules2();
        }
    }

    uint count_occupied() {
        return boost::count(state, '#');
    }

    void print() {
        for(auto it = state.begin(); it != state.end(); it += width) {
            cout << string(it, it + width) << endl;
        }
    }

    private:
    int width, height;
    string state, prev_state;
};

int main(int argc, char** argv) {
    auto s = get_input_all<seat_map>(string(argv[1]));
    auto s2 = s;

    s.stabilize(1);
    s2.stabilize(2);

    cout << s.count_occupied() << endl;
    cout << s2.count_occupied() << endl;

    return 0;
}
