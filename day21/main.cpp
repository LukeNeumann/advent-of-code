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


struct food_analyzer {
    food_analyzer(vector<string> lines) {
        for(auto& line : lines) {
            std::smatch m;
            std::regex_match(line, m, std::regex("(.*)\\(contains(.*)\\)"));
            vector<string> foods = split(strip(string(m[1]), ' '), ' ');
            vector<string> allergens = split(strip(string(m[2]), ' '), ',');
            std::sort(foods.begin(), foods.end());

            for(auto&f : foods)
                food_counts[f]++;

            for(auto& a : allergens) {
                a = strip(a,' ');

                if(allergen_narrowed[a].empty()) {
                    allergen_narrowed[a] = foods;
                } else {
                    vector<string> new_narrowed;
                    std::set_intersection(foods.begin(), foods.end(), allergen_narrowed[a].begin(), allergen_narrowed[a].end(), std::back_inserter(new_narrowed));
                    allergen_narrowed[a] = new_narrowed;
                }
            }
        }
    }

    vector<string> unfinished_allergens() {
        vector<string> result;
        for(auto& a : allergen_narrowed) {
            if(a.second.size() != 1)
                result.push_back(a.first);
        }
        return result;
    }

    void cleanup_mapped_foods() {
        for(auto& a : allergen_narrowed) {
            if(a.second.size() == 1) continue; // skip solved allergens
            for(auto& f : food_allergens) { // remove all mapped foods from unsolved allergens
                a.second.erase(std::remove(a.second.begin(), a.second.end(), f.first), a.second.end());
            }
        }
    }

    void update_mapped_foods() {
        for(auto& a : allergen_narrowed) {
            // if we've solved an allergen, but its food isn't mapped
            if(a.second.size() == 1 && food_allergens.find(a.second.front()) == food_allergens.end())
                food_allergens[a.second.front()] = a.first;
        }
    }

    void solve() {
        while(!unfinished_allergens().empty()) {
            cleanup_mapped_foods();
            update_mapped_foods();
        }
    }

    uint output1() {
        uint result = 0;
        for(auto& f : food_counts) {
            if(food_allergens.find(f.first) == food_allergens.end())
                result += f.second;
        }
        return result;
    }

    string output2() {
        vector<string> allergens_alphabetical;
        for(auto& a : allergen_narrowed)
            allergens_alphabetical.push_back(a.first);
        std::sort(allergens_alphabetical.begin(), allergens_alphabetical.end());

        std::ostringstream result;
        bool first = true;
        for(auto& a : allergens_alphabetical) {
            if(!first) result << ",";
            result << allergen_narrowed[a].front();
            if(first) first = false;
        }

        return result.str();

    }

    map<string, vector<string> > allergen_narrowed;
    map<string, string> food_allergens;
    map<string, uint> food_counts;
};

int main(int argc, char** argv) {
    auto lines = get_input_perline<string>(string(argv[1]));
    food_analyzer f(lines);
    f.solve();

    cout << f.output1() << endl;
    cout << f.output2() << endl;

    for(auto& a : f.allergen_narrowed) {
        cout << a.first << ": ";
        for(auto& food : a.second)
            cout << food << " ";
        cout << endl;
    }

    return 0;
}
