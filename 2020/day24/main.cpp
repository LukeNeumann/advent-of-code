#include <iostream>
#include <vector>
#include <queue>
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
#include <boost/dynamic_bitset.hpp>

#include "input_handler.hpp"

using std::vector;
using std::queue;
using std::string;
using std::map;
using std::pair;
using std::cout;
using std::endl;

struct hex_tile {
    hex_tile(vector<string> lines) {
        for(auto& line : lines)
            flip_tile(line);
    }

    void flip_tile(string directions) {
        //cout << directions << endl;
        pair<int, int> cur_tile{0,0};

        for(uint i = 0; i < directions.size(); ++i) {
            if(directions[i] == 'n' || directions[i] == 's') 
                cur_tile = get_adjacent(cur_tile, directions.substr(i++, 2));
            else 
                cur_tile = get_adjacent(cur_tile, directions.substr(i, 1));
        }

        blacks[cur_tile.first][cur_tile.second] = !blacks[cur_tile.first][cur_tile.second];

    }

    pair<int, int> get_adjacent(pair<int, int> tile, string dir) {
        auto result = tile;

        // columns: each a diagonal (up-right) chain of hex tiles
        if(dir == "e" || dir == "se") // east
            result.second++;
        if(dir == "w" || dir == "nw") // west
            result.second--;

        // rows: each a horizontal chain of hex tiles
        if(dir == "ne" || dir == "nw") // north
            result.first++;
        if(dir == "sw" || dir == "se") // south
            result.first--;

        //cout << "(" << tile.first << "," << tile.second << ")" << " "<<dir<<" " << "("<<result.first<<","<<result.second<<")" << endl;
        return result;
    }

    uint count() const {
        uint result = 0;
        for(const auto& it1 : blacks) {
            for(const auto& it2 : it1.second)
                result += it2.second;
        }
        return result;
    }

    uint count_adjacent(pair<int, int> tile) {
        uint result = 0;
        vector<string> directions{"e","w","ne","nw","se","sw"};
        for(auto& dir : directions) {
            auto adj = get_adjacent(tile, dir);
            result += blacks[adj.first][adj.second];
        }
        return result;
    }

    void update_tile(map<int, map<int, bool> >& new_blacks, pair<int, int> tile) {
        uint adj_count = count_adjacent(tile);
        //cout << "(" << tile.first << "," << tile.second << ") count=" << adj_count << endl;
        if(blacks[tile.first][tile.second]) { // black tile
            if(adj_count == 0 || adj_count > 2)
                new_blacks[tile.first][tile.second] = false;

            // update neighboring whites
            vector<string> directions{"e","w","ne","nw","se","sw"};
            for(auto& dir : directions) {
                auto adj = get_adjacent(tile, dir);
                //cout << "adj (" << adj.first << "," << adj.second << ")" << endl;
                if(!blacks[adj.first][adj.second]) { // neighboring white
                    uint adj_adj_count = count_adjacent(adj);
                    if(adj_adj_count == 2)
                        new_blacks[adj.first][adj.second] = true;
                }
            }
        } else { // white tile
            if(adj_count == 2)
                new_blacks[tile.first][tile.second] = true;
        }
        //cout << "updating (" << tile.first << "," << tile.second << ") from " << (blacks[tile.first][tile.second] ? "black" : "white") <<
        //                                                           " to " << (new_blacks[tile.first][tile.second] ? "black" : "white") << endl;
    }

    void daily_update() {
        auto new_blacks = blacks;
        for(const auto& it1 : blacks) {
            for(const auto& it2 : it1.second) {
                if(blacks[it1.first][it2.first])
                    update_tile(new_blacks, std::make_pair(it1.first, it2.first));
            }
        }
        blacks = new_blacks;
    }

    void update(uint n = 1) {
        for(uint i = 0; i < n; ++i) {
            //cout << "day " << i << endl;
            daily_update();
        }
    }

    map<int, map<int, bool> > blacks;
};

int main(int argc, char** argv) {
    auto lines = get_input_perline<string>(string(argv[1]));
    
    hex_tile h(lines);
    cout << h.count() << endl;

    h.update(100);
    cout << h.count() << endl;

    return 0;
}
