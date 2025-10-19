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

struct crypt_hack {
    crypt_hack(vector<string> lines) {
        keys.first = std::stoi(lines[0]);
        keys.second = std::stoi(lines[1]);

        loop_sizes.first = find_loop_size(keys.first);
        loop_sizes.second = find_loop_size(keys.second);
    }

    static ulong find_loop_size(ulong key) {
        ulong val = 1;
        for(uint i = 0; true; ++i) {
            val *= initial_subject;
            val = val % 20201227;
            if(val == key)
                return i + 1;
        }
        return 0;
    }

    static ulong transform(ulong subject, ulong loops) {
        ulong val = 1;
        for(uint i = 0; i < loops; ++i) {
            val *= subject;
            val = val % 20201227;
        }
        return val;
    }

    ulong get_encryption_key() const {
        return transform(keys.first, loop_sizes.second);
    }

    pair<ulong, ulong> keys;
    pair<ulong, ulong> loop_sizes;

    static constexpr ulong initial_subject = 7;
};

int main(int argc, char** argv) {
    auto lines = get_input_perline<string>(string(argv[1]));
    
    crypt_hack h(lines);
    cout << h.get_encryption_key() << endl;

    return 0;
}
