#include <iostream>
#include <vector>

using entry_t = uint;

// return a set of 2 entries such that the entry1 + entry2 = sum
// if no such set of entries exists, return 0,0
std::pair<entry_t, entry_t> find_sum_terms(std::vector<entry_t> entries, entry_t sum){
    std::vector<entry_t>::const_iterator it1, it2;
    for(it1 = entries.cbegin(); it1 != entries.cend(); ++it1) { 
        for(it2 = entries.cbegin(); it2 != entries.cend(); ++it2) { 
            if(it1 == it2) continue;
            if(((*it1) + (*it2)) == sum) return std::pair<entry_t, entry_t>{*it1, *it2};
        }
    }

    return std::pair<entry_t, entry_t>{0, 0};
}

int main(int argc, char** argv) {
    std::vector<entry_t> entries;

    for(size_t i = 1; i < argc; ++i) {
        std::string arg(argv[i]);
        entries.push_back(std::stoi(arg));    
    }

    auto terms = find_sum_terms(entries, 2020);
    std::cout << terms.first << " * " << terms.second << " = " << terms.first * terms.second << std::endl;

    return 0;
}
