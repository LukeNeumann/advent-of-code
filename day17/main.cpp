#include <iostream>
#include <vector>
#include <fstream>
#include <numeric>
#include <algorithm>
#include <map>
#include <iterator>
#include <functional>
#include <cmath>
#include <bitset>
#include <regex>
#include <boost/range/algorithm/count.hpp>

#include "input_handler.hpp"

using std::vector;
using std::string;
using std::map;
using std::pair;
using std::cout;
using std::endl;

struct energy_source {
    energy_source(vector<string> lines) {
        size = lines.size();
        // WEEEEEEEEE!!!!!!!
        cells = vector<vector<vector<bool> > >(size, vector<vector<bool> >(size,vector<bool>(size,false)));

        for(uint x = 0; x < size; x++) {
            for(uint y = 0; y < size; y++) {
                cells[x][y][0] = lines[y][x] == '#';
            }
        }
    }

    void increase_size() {
        for(uint x = 0; x < size; x++) {
            for(uint y = 0; y < size; y++) {
                cells[x][y].insert(cells[x][y].begin(),false);
                cells[x][y].push_back(false);
            }
            cells[x].insert(cells[x].begin(),vector<bool>(size+2, false));
            cells[x].push_back(vector<bool>(size+2, false));
        }

        cells.insert(cells.begin(),vector<vector<bool> >(size+2, vector<bool>(size+2, false)));
        cells.push_back(vector<vector<bool> >(size+2, vector<bool>(size+2, false)));

        size += 2;
    }

    void cycle() {
        increase_size();
        auto prev_cells = cells;
        for(uint x = 0; x < size; x++) {
            for(uint y = 0; y < size; y++) {
                for(uint z = 0; z < size; z++) {
                    cout << "  ";
                    uint num_active_neighbors = 0;
                    for(int xoff = ((x==0) ? 0 : -1); xoff <= ((x==size-1) ? 0 : 1); xoff++) {
                    for(int yoff = ((y==0) ? 0 : -1); yoff <= ((y==size-1) ? 0 : 1); yoff++) {
                    for(int zoff = ((z==0) ? 0 : -1); zoff <= ((z==size-1) ? 0 : 1); zoff++) {
                        if(xoff == 0 && yoff == 0 && zoff == 0) continue;
                        num_active_neighbors += prev_cells[x + xoff][y + yoff][z + zoff];
                        cout << x+xoff <<","<< y+yoff <<","<< z+zoff << " ";
                    }}}
                    cout << endl;

                    cout << x << "," << y << "," << z << " " << num_active_neighbors << endl;

                    if(prev_cells[x][y][z] == true) {
                        cells[x][y][z] = num_active_neighbors == 2 || num_active_neighbors == 3;
                    } else {
                        cells[x][y][z] = num_active_neighbors == 3;
                    }
                }
            }
        }
    }

    void cycle(uint count) {
        for(uint i = 0; i < count; i++) cycle();
    }

    uint count_active_cells() {
        uint sum = 0;
        for(uint x = 0; x < size; x++) {
            for(uint y = 0; y < size; y++) {
                for(uint z = 0; z < size; z++) {
                    sum += cells[x][y][z];
                }
            }
        }
        return sum;
    }

    private:
    vector<vector<vector<bool> > > cells;
    uint size;
};

int main(int argc, char** argv) {
    auto e = get_input_all<energy_source>(string(argv[1]));

    e.cycle(6);

    cout << e.count_active_cells() << endl;

    return 0;
}
