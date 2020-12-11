#include <iostream>
#include <vector>
#include <fstream>
#include <numeric>
#include <algorithm>
#include <map>
#include <iterator>

#include "input_handler.hpp"

using std::vector;
using std::string;
using std::map;
using std::pair;
using std::cout;
using std::endl;

struct adapter {
    adapter(string line) { rating = stoi(line); }
    adapter(uint r) : rating(r) {}

    bool connects(const adapter input) const {
        return (int)input >= (int)rating + input_range.first &&
               (int)input <= (int)rating + input_range.second;
    }

    operator uint() const { return rating; }

    uint rating;

    private:
    static pair<int, int> input_range;
};

struct chain {
    int add_adapter(adapter a) {
        if(!a.connects(adapters.back())) {
            //cout << "Can't connect adapter " << a << " to " << adapters.back() << endl;
            return 1;
        }

        jolt_diffs[a - adapters.back()]++;
        adapters.push_back(a);
        return 0;
    }

    int add_device(adapter device) {
        return add_adapter(device + device_diff);
    }

    void add_plug() {
        adapters.push_back(adapter(0));
    }

    vector<adapter> adapters;
    map<uint, uint> jolt_diffs;
    static uint device_diff;
};

pair<int,int> adapter::input_range{-3, 0};
uint chain::device_diff{3};

ulong count_arrangements(const vector<adapter> adapters) {
    map<adapter, ulong> connect_counts;
    for(auto it1 = adapters.cbegin(); it1 != adapters.end(); ++it1) {
        for(auto it2 = it1 + 1; it2 != adapters.end() && it2->connects(*it1); ++it2) 
            connect_counts[*it1]++;
    }

    map<adapter, ulong> arrangement_counts;
    arrangement_counts[adapters.back()] = 1;
    for(auto it1 = adapters.crbegin() + 1; it1 != adapters.crend(); ++it1) {
        for(auto it2 = it1 - connect_counts[*it1]; it2 != it1; ++it2)
            arrangement_counts[*it1] += arrangement_counts[*it2];
    }

    return arrangement_counts[adapters.front()];
}

int main(int argc, char** argv) {
    auto entries = get_input_perline<adapter>(string(argv[1]));

    std::sort(entries.begin(), entries.end());

    chain c;
    c.add_plug();
    for(auto& e : entries) {
        c.add_adapter(e);
    }
    c.add_device(entries.back());

    cout << c.jolt_diffs[1] * c.jolt_diffs[3] << endl;
    cout << count_arrangements(c.adapters) << endl;

    return 0;
}
