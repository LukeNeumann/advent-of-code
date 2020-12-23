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

struct cup_game {
    cup_game(vector<string> lines) {
        for(auto& c : lines[0]) {
            uint cup = c - '0';
            if(cup > max_cup) max_cup = cup;
            circle.push_back(cup);
        }
        //cout << "max cup " << max_cup << endl;
    }

    string to_string() {
        std::ostringstream result;
        for(auto& c : circle) {
            if(c == *cur) 
                result << "(" << c << ")";
            else
                result << c;
        }
        return result.str();
    }

    void circular_advance(std::list<uint>::iterator& it, uint n = 1) {
        for(uint i = 0; i < n; ++i) {
            it++;
            if(it == circle.end()) it = circle.begin();
        }
    }

    void move_circle(const std::vector<uint>& removed, std::list<uint>::iterator dest) {
        uint distance = std::distance(circle.begin(), cur);

        // insert after the destination
        circular_advance(dest); 
        
        // move the 3 elements after cur
        auto source_start = cur;
        circular_advance(source_start);
        auto source_end = source_start;
        circular_advance(source_end, 3);

        uint cur_val = *cur;
        //cout << "cur_val " << cur_val << endl;

        circle.splice(dest, circle, source_start, source_end);

        // fix the cur iterator
        cur = std::find(circle.begin(), circle.end(), cur_val);
        circular_advance(cur);
    }

    std::list<uint>::iterator get_dest_index(const std::vector<uint>& removed) {
        uint dest_label = (*cur) - 1;
        if(dest_label == 0) dest_label = max_cup;
        while(find(removed.begin(), removed.end(), dest_label) != removed.end()) {
            if(dest_label == 1) dest_label = max_cup; 
            else dest_label--;
        }

        //cout << "  dest " << dest_label << endl;
        return find(circle.begin(), circle.end(), dest_label);
    }

    void play(uint turns = 1) {
        cur = circle.begin();
        for(uint t = 0; t < turns; ++t) {
            //cout << "turn " << t << " cups: " << to_string() << endl;
            // remove the 3 cups after current
            vector<uint> removed; 
            auto it = cur;
            //cout << "pick up:";
            for(uint i = 0; i < 3; ++i) {
                circular_advance(it);
                removed.push_back(*it);
                //cout << " " << *it;
            }
            //cout << endl;

            // find the destination cup
            auto dest_it = get_dest_index(removed);
            //cout << "destination: " << *dest_it << endl;

            // insert the 3 cups after the destination cup
            move_circle(removed, dest_it);

            cout << 100 * t / turns << "% t=" << t << endl;
        }
    }

    protected:
    std::list<uint> circle; // ordered circle of cups
    uint max_cup = 0;
    std::list<uint>::iterator cur;
};

struct cup_game_v2 : public cup_game {
    cup_game_v2(vector<string> lines) : cup_game(lines) {
        for(uint c = max_cup + 1; c <= 1000000; ++c) {
            if(c > max_cup) max_cup = c;
            circle.push_back(c);
        }
    }

    ulong output2() {
        ulong result = 1;
        auto it = find(circle.begin(), circle.end(), 1);
        for(uint i = 0; i < 2; ++i)
            result *= *(++it);
        return result;
    }
};

int main(int argc, char** argv) {
    auto lines = get_input_perline<string>(string(argv[1]));
    
    cup_game c1(lines);
    c1.play(100);
    cout << c1.to_string() << endl;

    cup_game_v2 c2(lines);
    c2.play(10000000);
    cout << c2.output2() << endl;

    return 0;
}
