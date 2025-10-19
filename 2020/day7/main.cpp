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

struct rule {
    rule() {}
    rule(string line) {
        std::smatch m;
        std::regex_match(line, m, std::regex("(.*) bags contain(.*)"));
        bag_type = m[1];

        string contains = m[2];

        string::iterator start = contains.begin();
        string::iterator sep = std::find_if(contains.begin(), contains.end(), [](char a) { return a == ',' || a == '.'; });
        while(sep != contains.end()){
            string section(start, sep);
            if(std::regex_match(section, m, std::regex(" ([1-9]+) (.*) bags?")))
                bag_contains[m[2]] = std::stoi(m[1]);

            start = sep + 1;
            sep = std::find_if(start, contains.end(), [](char a) { return a == ',' || a == '.'; });
        }
    }

    string bag_type;
    // list of (bag_type, #) of bags contained
    map<string, uint> bag_contains;
};

bool can_contain(const map<string, rule>& rule_map, const string current, string target) {
    for(auto const & c : rule_map.at(current).bag_contains) {
        if(c.first == target) return true;
    }

    for(auto const & c : rule_map.at(current).bag_contains) {
        if(can_contain(rule_map, c.first, target)) return true; 
    }

    return false;
}

uint num_bags_inside(const map<string, rule>& rule_map, const string current) {
    if(rule_map.at(current).bag_contains.empty()) return 0;

    uint num = 0;
    for(auto const & c : rule_map.at(current).bag_contains) {
        num += (1 + num_bags_inside(rule_map, c.first)) * c.second;
    }

    return num;
}

using entry_t = rule;
int main(int argc, char** argv) {
    vector<entry_t> rules;

    std::ifstream input_file{string(argv[1])};
    if(input_file.is_open()) {
        string line;
        while(getline(input_file,line)) {
            rules.push_back(entry_t(line));
        }
    } else {
        std::cout << "Failed to open input file" << std::endl;
    }

    map<string, rule> rule_map;
    for(auto& r : rules) {
        rule_map[r.bag_type] = r;
    }

    string target = "shiny gold";
    uint count = 0;
    for(auto& node : rule_map) {
        count += can_contain(rule_map, node.first, target);
    }

    cout << count << endl;
    cout << num_bags_inside(rule_map, target) << endl;
        

    return 0;
}
