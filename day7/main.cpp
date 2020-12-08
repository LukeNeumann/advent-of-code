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

bool can_contain(const map<string, vector<pair<string, uint> > >& contains_graph, const string current, string target) {
    for(auto const & c : contains_graph.at(current)) {
        if(c.first == target) return true;
    }

    for(auto const & c : contains_graph.at(current)) {
        if(can_contain(contains_graph, c.first, target)) return true; 
    }

    return false;
}

uint num_bags_inside(const map<string, vector<pair<string, uint> > >& contains_graph, const string current) {
    if(contains_graph.at(current).empty()) return 0;

    uint num = 0;
    for(auto const & c : contains_graph.at(current)) {
        num += (1 + num_bags_inside(contains_graph, c.first)) * c.second;
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

    map<string, vector<pair<string, uint> > > contains_graph;
    for(auto& r : rules) {
        contains_graph[r.bag_type] = vector<pair<string, uint> >();
        for(auto& c : r.bag_contains) {
            contains_graph[r.bag_type].push_back(c);
        }
    }

    string target = "shiny gold";
    uint count = 0;
    for(auto& node : contains_graph) {
        count += can_contain(contains_graph, node.first, target);
    }

    cout << count << endl;
    cout << num_bags_inside(contains_graph, target) << endl;
        

    return 0;
}
