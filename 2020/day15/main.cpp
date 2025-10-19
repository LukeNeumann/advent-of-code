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

struct number_info {
    pair<ulong,ulong> occurence;
    bool has_multiple = false;
};

struct memory_game {
    memory_game(vector<string> lines) {
        string s = lines.front();
        cout << s << endl;
        auto prev_sep = s.begin();
        auto sep = find(s.begin(), s.end(), ',');
        while(sep != s.end()) {
            start_sequence.push_back(stoi(string(prev_sep,sep)));
            prev_sep = sep + 1;
            sep = find(prev_sep, s.end(), ',');
        }
        start_sequence.push_back(stoi(string(prev_sep,sep)));
    }

    ulong play(ulong turns) {
        numbers.clear();
        seq_size = 0;

        ulong result = 0;
        for(ulong i = 1; i <= turns; ++i) {
            ulong next_number = 0;

            if(seq_size < start_sequence.size()) {
                next_number = start_sequence[seq_size]; 
            } else if (!numbers[result].has_multiple) {
                next_number = 0;
            } else {
                auto prev = numbers[result];
                next_number = prev.occurence.second - prev.occurence.first;
            }

            if(numbers.find(next_number) != numbers.end()) numbers[next_number].has_multiple = true;
            numbers[next_number].occurence.first = numbers[next_number].occurence.second;
            numbers[next_number].occurence.second = ++seq_size;
            result = next_number;
            //cout << result << endl;

            //if(i % (turns / 100) == 0)
                //cout << 100 * i / turns << "%" << endl; 
        }

        return result;
    }

    private: 
    std::unordered_map<ulong, number_info> numbers;
    vector<ulong> start_sequence;
    ulong seq_size;
};

int main(int argc, char** argv) {
    auto lines = get_input_perline<string>(string(argv[1]));

    memory_game m(lines);

    cout << m.play(2020) << endl;
    cout << m.play(30000000) << endl;

    return 0;
}
