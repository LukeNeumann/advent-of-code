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

struct rule {
    rule() = default;
    rule(uint i, string m) { 
        id = i; leaf = false;
        for(auto& group : split(m, '|')) {
            vector<uint> rule_ids;
            for(auto& r : split(strip(group,' '), ' ')) {
                rule_ids.push_back(stoi(r));
            }
            sub_rules.push_back(rule_ids);
        }
    }
    rule(uint i, char m) { 
        id = i; leaf = true;
        match = m;
    }

    string to_string() const { 
        if(leaf)
            return string(1,match); 

        std::ostringstream result;
        bool first_group = true;
        for(const auto& group : sub_rules) {
            result << (first_group ? "" : "|");
            first_group = false;
            bool first_rule = true;
            for(const auto& rule : group) {
                result << (first_rule ? "" : ",") << rule;
                first_rule = false;
            }
        }
        return result.str();
    }

    bool leaf; // true is this is a leaf_rule
    char match; // for leaf rules
    vector<vector<uint> > sub_rules; // for branch rules
    uint id;
};

struct message_checker {
    // return a list of substrings of m, each of which matches the rule in rules[r]
    uint check(string m, uint r) const {
        uint result = 0;

        if(rules.at(r).leaf) {
            if(rules.at(r).match == m[0])
                result = 1; // 1 match, the character of the leaf
        } else {
            for(const auto& group : rules.at(r).sub_rules) {
                uint used = 0;
                bool group_passed = true;
                for(const auto& sub_rule : group) {
                    string sub_m(m.begin() + used, m.end()); 
                    uint sub_matches = check(sub_m, sub_rule);
                    if(sub_matches != 0) {
                        used += sub_matches;
                        //cout << "sub_rule " << sub_rule << "; sub_matches[0] " << sub_matches[0] << endl;
                    } else {
                        group_passed = false;
                    }
                }
                if(group_passed)
                    result = used;
            }
        }

        return result;
    }

    uint check0_part2(string m) {
        //8: 42 | 42 8
        //11: 42 31 | 42 11 31  
        //0: 8 11
        //0 in english: n 42s then m 31s where n > m and n >= 1 and m >= 1
        //greedy
        uint count_42s = 0;
        uint sub = check(m, 42);
        if(sub == 0) return 0;

        uint result = sub;
        while(sub > 0) {
            count_42s++;
            sub = check(string(m.begin() + result, m.end()), 42);
            result += sub;
        }
        //cout << "42s " << count_42s << endl;
        if(count_42s < 2) return 0;

        uint count_31s = 0;
        sub = check(string(m.begin() + result, m.end()), 31);
        result += sub;
        if(sub == 0) return 0;
        while(sub > 0) {
            count_31s++;
            if(count_31s == count_42s - 1) break;
            sub = check(string(m.begin() + result, m.end()), 31);
            result += sub;
        }
        //cout << "31s " << count_31s << endl;

        return result;
    }


    rule rule_factory(string line) {
        std::smatch m;
        std::regex_match(line, m, std::regex("([0-9]+): (.*)"));
        uint id = stoi(string(m[1]));
        auto rule_string = string(m[2]);

        if(std::regex_match(rule_string, m, std::regex("\"([a-z])\""))) {
            return rule(id, string(m[1])[0]); 
        }

        return rule(id,rule_string);
    }

    void input(vector<string> lines) {
        bool is_rule = true;
        for(auto& line : lines) {
            if(line.empty()) {
                is_rule = false;
                continue;
            }
            if(is_rule) {
                auto r = rule_factory(line);
                rules[r.id] = r;
            } else {
                messages.push_back(line);
            }
        }

        for(auto& r : rules) {
            //cout << r.first << ": " << r.second.to_string() << endl;
        }
    }

    uint output() {
        uint count = 0;
        for(auto& m : messages) {
            uint matches;
            if(part2)
                matches = check0_part2(m);
            else
                matches = check(m, 0); 
            bool result = matches == m.size();
            //cout << m << ": " << result << endl;
            count += result;
        }
        return count;
    }

    bool part2 = false;
    map<uint, rule> rules;
    vector<string> messages;
};

int main(int argc, char** argv) {
    auto lines = get_input_perline<string>(string(argv[1]));

    message_checker m;
    m.input(lines);

    cout << m.output() << endl;
    m.part2 = true;
    cout << m.output() << endl;

    return 0;
}
