#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <stdexcept>

using entry_t = uint;

// return a vector of n entries such that the sum of n entries = sum
// if no such set of entries exists, return an empty vector
std::vector<entry_t> find_sum_terms(std::vector<entry_t> entries, entry_t sum, size_t n_entries = 2, 
                                    std::vector<entry_t> results = std::vector<entry_t>()){

    // base case
    if(n_entries == 0) {
        if(sum == 0) { // success!
            return results;
        } else { 
            return std::vector<entry_t>();
        }
    }

    for(int i = 0; i < entries.size(); i++) {
        std::vector<entry_t> new_entries, new_results;

        new_entries = entries;
        new_entries.erase(new_entries.begin() + i);

        new_results = results;
        new_results.push_back(entries[i]);

        auto answer = find_sum_terms(new_entries, sum - entries[i], n_entries - 1, new_results);
        if(answer.size() != 0)
            return answer;
    }

    return std::vector<entry_t>();
}

// print out the terms and their product
void print_terms(std::vector<entry_t> terms) {
    uint i = 0;
    for(auto& term : terms) {
        std::cout << (i++ == 0 ? "" : " * ");
        std::cout << term;
    }
    if(terms.size() > 0)
        std::cout << " = " << std::accumulate(terms.cbegin(), terms.cend(), 1, [](entry_t a, entry_t b) { return a * b; }) << std::endl;
}

int main(int argc, char** argv) {
    std::vector<entry_t> entries;

    std::ifstream input_file{std::string(argv[1])};
    if(input_file.is_open()) {
        std::string line;
        while(getline(input_file,line)) {
            line.erase(std::find_if(line.begin(), line.end(), [](char c) { return c == ' '; } ), 
                       line.end());
            try {
                entries.push_back(std::stoi(line, nullptr, 10));    
            } catch(std::invalid_argument& e) {}
        }
    } else {
        std::cout << "Failed to open input file" << std::endl;
    }

    print_terms(find_sum_terms(entries, 2020, 2));
    print_terms(find_sum_terms(entries, 2020, 3));

    return 0;
}
