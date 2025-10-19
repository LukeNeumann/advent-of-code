#include <iostream>
#include <vector>
#include <fstream>
#include <numeric>
#include <algorithm>
#include <regex>

using std::vector;
using std::string;
using std::map;
using std::pair;
using std::cout;
using std::endl;

struct XMAS_cypher {
    XMAS_cypher(vector<ulong> entries) : numbers(entries) {}

    bool check_valid(ulong index) {
        if(index < 25) return true;
        for(size_t i = index - 25; i < index; i++) {
            for(size_t j = index - 25; j < index; j++) {
                if(i != j && numbers[i] + numbers[j] == numbers[index])
                    return true;
            }
        }
        return false;        
    }

    ulong find_failure() {
        for(size_t i = 0; i < numbers.size(); i++) {
            if(!check_valid(i)) return numbers[i];
        }

        return 0;
    }

    pair<vector<ulong>::iterator, vector<ulong>::iterator> find_set_bounds(ulong target) {
        for(size_t set_size = 2; set_size < numbers.size(); ++set_size) {
            for(vector<ulong>::iterator it1 = numbers.begin(); it1 + set_size != numbers.end(); ++it1) {
                ulong sum = 0;
                for(vector<ulong>::iterator it2 = it1; it2 != it1 + set_size; ++it2) {
                    sum += *it2;
                    if(sum > target) break;
                }
                if(sum == target) return pair<vector<ulong>::iterator, vector<ulong>::iterator>(it1, it1 + set_size);
            }
        }
        
        return pair<vector<ulong>::iterator, vector<ulong>::iterator>(numbers.begin(), numbers.begin());
    }

    ulong break_cypher() {
        ulong failure = find_failure();
        if(!failure) return 0;

        pair<vector<ulong>::iterator, vector<ulong>::iterator> set_bounds = find_set_bounds(failure);
        if(set_bounds.first == numbers.begin() && set_bounds.second == numbers.begin()) return 0;

        ulong max = std::accumulate(set_bounds.first, set_bounds.second, *set_bounds.first, [](ulong a, ulong b) { return std::max(a, b); });
        ulong min = std::accumulate(set_bounds.first, set_bounds.second, *set_bounds.first, [](ulong a, ulong b) { return std::min(a, b); });

        return max + min;
    }

    private:
    vector<ulong> numbers;
};

using entry_t = ulong;
int main(int argc, char** argv) {
    vector<entry_t> entries;

    std::ifstream input_file{string(argv[1])};
    if(input_file.is_open()) {
        string line;
        while(getline(input_file,line)) {
            entries.push_back(stoul(line));
        }
    } else {
        std::cout << "Failed to open input file" << std::endl;
    }

    XMAS_cypher c(entries);
    cout << c.find_failure() << endl;
    cout << c.break_cypher() << endl;

    return 0;
}
