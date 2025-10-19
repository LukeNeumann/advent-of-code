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
#include <boost/dynamic_bitset.hpp>

#include "input_handler.hpp"

using std::vector;
using std::string;
using std::map;
using std::pair;
using std::cout;
using std::endl;


struct tile {
    tile() = default;
    tile(uint i, vector<string> lines) : id(i) {
        tilesize = lines.size();
        for(auto& line : lines) {
            vector<bool> row;
            for(uint i = 0; i < line.size(); i++) 
                row.push_back(line[i] == '#');
            map.push_back(row);
        }

        for(uint s = 0; s < 4; ++s) 
            sides.push_back(get_side_sig(s));
    }
    
    string to_string() const {
        std::ostringstream result;
        result << id << endl;
        for(auto& row : map) {
            for(auto cell : row) {
                result << cell;
            }
            result << endl;
        }

        return result.str();
    }

    string sides_string() const {
        std::ostringstream result;
        for(uint s = 0; s < 4; ++s)
            result << s << ": " << std::hex << sides[s] << std::dec << ", ";
        return result.str();
    }

    uint get_side_sig(uint side) {
        boost::dynamic_bitset<> result(tilesize);
        if(side == 0) {// N
            for(uint j = 0; j < tilesize; ++j)
                result[tilesize - 1 - j] = map[0][j];
        } else if(side == 2) {// S
            for(uint j = 0; j < tilesize; ++j)
                result[tilesize - 1 - j] = map[tilesize - 1][j];
        } else if(side == 1) {// E
            for(uint i = 0; i < tilesize; ++i)
                result[tilesize - 1 - i] = map[i][tilesize - 1];
        } else {// W
            for(uint i = 0; i < tilesize; ++i)
                result[tilesize - 1 - i] = map[i][0];
        }
        return result.to_ulong();
    }

    uint reverse(uint s) const {
        boost::dynamic_bitset<> sig(tilesize, s);
        boost::dynamic_bitset<> rsig(tilesize);
        for(uint i = 0; i < tile::tilesize; ++i) {
            rsig[i] = sig[tile::tilesize - i - 1];
        }
        return rsig.to_ulong();
    }

    void fliph() {
        sides[0] = reverse(sides[0]);
        sides[2] = reverse(sides[2]);
        std::swap(sides[1], sides[3]);

        //cout << "pre fliph:" << endl << to_string() << endl;
        for(uint i = 0; i < tilesize; ++i) {
        for(uint j = 0; j < tilesize / 2; ++j) {
            std::swap(map[i][j], map[i][tilesize - 1 - j]);
        }
        }
        //cout << "post fliph:" << endl << to_string() << endl;

    }
    void flipv() {
        sides[1] = reverse(sides[1]);
        sides[3] = reverse(sides[3]);
        std::swap(sides[0], sides[2]);

        //cout << "pre flipv:" << endl << to_string() << endl;
        for(uint j = 0; j < tilesize; ++j) {
        for(uint i = 0; i < tilesize / 2; ++i) {
            std::swap(map[i][j], map[tilesize - 1 - i][j]);
        }
        }
        //cout << "post flipv:" << endl << to_string() << endl;
    }
    void rotate(uint num) {
        //cout << id << " rotating " << num << endl;
        for(uint n = 0; n < num % 4; ++n) {
            vector<uint> new_sides(4,0);
            for(uint side = 0; side < 4; ++side) {
                uint new_side = sides[side];
                if(side == 1 || side == 3)
                    new_side = reverse(new_side); 
                new_sides[(side+1) % 4] = new_side;
            }
            sides = new_sides;

            //cout << "pre rotate:" << endl << to_string() << endl;
            vector<vector<bool> > new_map = map;
            for(uint i = 0; i < tilesize; ++i) {
            for(uint j = 0; j < tilesize; ++j) {
                uint newi = j;
                uint newj = tilesize-1-i;
                new_map[newi][newj] = map[i][j];
            }
            }
            map = new_map;
            //cout << "post rotate:" << endl << to_string() << endl;
        }
    }

    uint count() {
        uint result = 0;
        for(auto& row : map) 
            result += std::accumulate(row.begin(), row.end(), 0);
        return result;
    }

    uint tilesize;
    uint id;
    vector<vector<bool> > map;
    vector<uint> sides;
};


struct tile_assembler {
    void input(vector<string> lines) {
        vector<string> tile_lines;
        uint tile_id;
        for(auto& line : lines) {
            std::smatch m;
            if(line.empty()) {
                tiles[tile_id] = tile(tile_id, tile_lines);
                //cout << tiles[tile_id].to_string() << endl;
            } else if(std::regex_match(line, m, std::regex("Tile ([0-9]+):"))) {
                tile_id = stoi(string(m[1]));
                tile_lines.clear();
            } else {
                tile_lines.push_back(line);
            }
        }
        tmapsize = sqrt(tiles.size());
    }

    bool sides_match(uint s1, uint s2) const {
        if(s1 == s2) return true;
        if(tiles.begin()->second.reverse(s1) == s2) return true;

        return false;
    }

    void add_side_sig(uint signature) {
        if(side_sigs.find(signature) != side_sigs.end()) {
            side_sigs[signature]++;
            return;
        }

        uint rsig = tiles.begin()->second.reverse(signature);
        if(side_sigs.find(rsig) != side_sigs.end()) {
            side_sigs[rsig]++;
            return;
        }

        side_sigs[signature]++;
    }

    pair<uint, uint> find_match(uint s1, vector<uint> used_tiles) const {
        for(auto& t2 : tiles) {
            if(find(used_tiles.begin(), used_tiles.end(), t2.second.id) != used_tiles.end()) continue;
            for(uint s = 0; s < 4; ++s) {
                if(sides_match(s1, t2.second.sides[s])) {
                    cout << "match found!! " << t2.second.id << " " << s << " " << std::hex << s1 << " " << t2.second.sides[s] << std::dec << endl;
                    return pair<uint,uint>{t2.second.id,s};
                }
            }
        }
        cout << "WARN: couldn't find match for " << std::hex << s1 << std::dec << endl;
        for(auto& used : used_tiles) {
            for(uint s = 0; s < 4; ++s) {
                if(sides_match(s1, tiles.at(used).sides[s])) {
                    cout << "WARN: used tile matches!! " << used << " " << s << " " << std::hex << s1 << " " << tiles.at(used).sides[s] << std::dec << endl;
                }
            }
        }

        return pair<uint,uint>{0,0};
    }

    void connect_tiles(pair<uint,uint> t1, pair<uint, uint> t2) {
        uint connect_target_side = (t1.second + 2) % 4;
        //cout << " connect target side = " << connect_target_side << endl;
        //cout << " pre-orientation " << t2.first << ": " << tiles.at(t2.first).sides_string() << endl;
        tiles[t2.first].rotate((connect_target_side - t2.second) % 4);
        //cout << " post-rotate " << t2.first << ": " << tiles.at(t2.first).sides_string() << endl;
        if(tiles[t1.first].sides[t1.second] != tiles.at(t2.first).sides[connect_target_side]) {
            if(connect_target_side % 2 == 0)
                tiles.at(t2.first).fliph();
            else
                tiles.at(t2.first).flipv();
        }
        //cout << " post-flip " << t2.first << ": " << tiles.at(t2.first).sides_string() << endl;
    }

    // the input sides only have 1 match, making this easier. thanks aoc!
    void assemble() {
        // find a corner
        uint t = tiles.begin()->second.id;
        vector<uint> used_tiles{t};
        while(true) { // find the top edge
            cout << "finding top, " << t<< endl;
            uint search_side = tiles.at(t).sides[0];
            auto m = find_match(search_side, used_tiles);
            if(m.first == 0) break; 
            connect_tiles(std::make_pair(t, 0), m);
            t = m.first;
            used_tiles.push_back(t);

        }
        while(true) { // find the left edge
            cout << "finding left, " << t<< endl;
            uint search_side = tiles.at(t).sides[3];
            auto m = find_match(search_side, used_tiles);
            if(m.first == 0) break; 
            connect_tiles(std::make_pair(t, 3), m);
            t = m.first;
            used_tiles.push_back(t);
        }
        cout << "Found a corner: " << t << endl;

        tmap = vector<vector<uint> >(tmapsize, vector<uint>());

        // fill in each row
        for(uint row = 0; row < tmapsize; ++row) {
            if(row == 0) {
                tmap[row].push_back(t);
                used_tiles = vector<uint>(1, t);
            } else {
                // match the previous row, column 0
                uint prev_row = tmap[row - 1][0];
                auto m = find_match(tiles[prev_row].sides[2], used_tiles);
                connect_tiles(std::make_pair(prev_row, 2), m);
                used_tiles.push_back(m.first);
                tmap[row].push_back(m.first);
            }

            for(uint col = 1; col < tmapsize; ++col) {
                uint prev_col = tmap[row][col - 1];
                cout << "searching for col " << col << " of row " << row << " which previously added " << prev_col << endl;
                auto m = find_match(tiles[prev_col].sides[1], used_tiles);
                connect_tiles(std::make_pair(prev_col, 1), m);
                used_tiles.push_back(m.first);
                tmap[row].push_back(m.first);
            }
        }
    }

    ulong output1() {
        //for(uint ti = 0; ti < tmapsize; ++ti) {
        //for(uint i = 0; i < tile::tilesize; ++i) {
        //    for(uint tj = 0; tj < tmapsize; ++tj) {
        //    for(uint j = 0; j < tile::tilesize; ++j) {
        //        cout << tiles[tmap[ti][tj]].map[i][j];
        //    }
        //    cout << " ";
        //    }
        //    cout << endl;
        //}
        //cout << endl;
        //}
        cout << tmap[0][0] <<" * "<< tmap[0][tmapsize-1] <<" * "<< tmap[tmapsize-1][0] <<" * "<< tmap[tmapsize-1][tmapsize-1] << endl;
        return ulong(tmap[0][0]) * ulong(tmap[0][tmapsize-1]) * ulong(tmap[tmapsize-1][0]) * ulong(tmap[tmapsize-1][tmapsize-1]);
    }

    bool sea_monster_match(const tile fmap, uint i, uint j) {
        vector<vector<bool> > sea_monster; // has 15 #'s
        //                  # 
        //#    ##    ##    ###
        // #  #  #  #  #  #   
        sea_monster.push_back(vector<bool>{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0}); 
        sea_monster.push_back(vector<bool>{1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,1});
        sea_monster.push_back(vector<bool>{0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0});

        for(uint si = 0; si < sea_monster.size(); ++si) {
        for(uint sj = 0; sj < sea_monster.front().size(); ++sj) {
            if(i+si >= fmap.tilesize || j+sj >= fmap.tilesize) 
                return false;
            if(sea_monster[si][sj] && !fmap.map[i+si][j+sj])
                return false;
        }
        }

        return true;
    }

    uint count_sea_monsters(const tile fmap) {
        uint result = 0;
        for(uint i = 0; i < fmap.tilesize; ++i) {
        for(uint j = 0; j < fmap.tilesize; ++j) {
            result += sea_monster_match(fmap, i, j); 
        }
        }
        return result;
    }

    uint output2() {
        uint tsize = tiles.begin()->second.tilesize;
        uint fmapsize = (tsize - 2) * tmapsize;
        tile fmap;
        fmap.tilesize = fmapsize;
        fmap.sides = vector<uint>(4,0);
        fmap.map = vector<vector<bool> >(fmapsize, vector<bool>(fmapsize, false));

        for(uint ti = 0; ti < tmapsize; ++ti) {
        for(uint tj = 0; tj < tmapsize; ++tj) {
            for(uint i = 1; i < tsize-1; ++i) {
            for(uint j = 1; j < tsize-1; ++j) {
                fmap.map[ti * (tsize-2) + i - 1][tj * (tsize-2) + j - 1] = tiles[tmap[ti][tj]].map[i][j];
            }
            }
        }
        }

        uint num_sea_monsters;
        uint num_waves = fmap.count();
        for(uint rotate = 0; rotate < 4; ++rotate) {
            num_sea_monsters = count_sea_monsters(fmap);
            if(num_sea_monsters != 0) return num_waves - 15 * num_sea_monsters;

            fmap.flipv();
            num_sea_monsters = count_sea_monsters(fmap);
            if(num_sea_monsters != 0) return num_waves - 15 * num_sea_monsters;
            fmap.flipv();

            fmap.fliph();
            num_sea_monsters = count_sea_monsters(fmap);
            if(num_sea_monsters != 0) return num_waves - 15 * num_sea_monsters;
            fmap.fliph();

            fmap.fliph();
            fmap.flipv();
            num_sea_monsters = count_sea_monsters(fmap);
            if(num_sea_monsters != 0) return num_waves - 15 * num_sea_monsters;
            fmap.flipv();
            fmap.fliph();
            
            fmap.rotate(1);
        }

        return 0;
    }

    vector<tile> tiles_list;
    vector<vector<uint> > tmap;
    uint tmapsize;
    map<uint, tile> tiles;
    map<uint, uint> side_sigs;
};

int main(int argc, char** argv) {
    auto lines = get_input_perline<string>(string(argv[1]));

    tile_assembler t;
    t.input(lines);
    t.assemble();

    cout << t.output1() << endl;
    cout << t.output2() << endl;

    for(auto& ti : t.tiles) {
        for(uint s = 0; s < 4; ++s)
            t.add_side_sig(ti.second.get_side_sig(s));
    }

    //for(auto& sig : t.side_sigs) {
    //    cout << sig.first << " " << sig.second << endl;
    //}

    return 0;
}
