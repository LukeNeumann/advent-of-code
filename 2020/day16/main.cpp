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

struct ticket_decoder {
    using ticket_t = vector<uint>;
    using rule_t = vector<pair<uint, uint> >;

    ticket_decoder(vector<string> lines) {
        bool my_ticket;
        bool is_rule = true;

        for(auto& line : lines) {
            if(line.find("your ticket") != string::npos) {
                is_rule = false;
                my_ticket = true;
            } else if(line.find("nearby tickets") != string::npos) {
                is_rule = false;
                my_ticket = false;
            } else if(!line.empty()) {
                if(is_rule) add_rule(line);
                else add_ticket(line, my_ticket);
            }
        }
    }

    void add_rule(string line) {
        std::smatch m;
        std::regex_match(line, m, std::regex("(.*): ([0-9]+)-([0-9]+) or ([0-9]+)-([0-9]+)"));
        string rulename(m[1]);
        std::pair<uint, uint> range1(stoi(string(m[2])), stoi(string(m[3])));
        std::pair<uint, uint> range2(stoi(string(m[4])), stoi(string(m[5])));

        rules[rulename] = rule_t{range1, range2};
        //cout << "rules[" << rulename << "] = (" << range1.first << ", " << range1.second << "), (" << range2.first << ", " << range2.second << ")" << endl;
    }

    void add_ticket(string line, bool mine = false) {
        ticket_t t;
        for(auto& s : split(line, ','))
            t.push_back(stoi(s));
        if(mine) {
            ticket = t;
            //cout << "my ticket: ";
            //for(auto& n : t) 
            //    cout << n << ", ";
            //cout << endl;
        }
        else {
            nearby_tickets.push_back(t);
            //cout << "nearby ticket: ";
            //for(auto& n : t) 
            //    cout << n << ", ";
            //cout << endl;
        }
    }

    bool rule_check(rule_t r, uint v) {
        for(auto& range : r) {
            if(v >= range.first && v <= range.second) return true; 
        }
        return false;
    }

    bool is_completely_invalid(uint v) {
        for(auto & rule : rules) {
            if(rule_check(rule.second, v)) return false;
        }
        return true;
    }

    uint output1() {
        uint sum = 0;
        for(auto& t : nearby_tickets) {
            bool is_valid_ticket = true;
            for(auto& v : t) {
                if(is_completely_invalid(v)) {
                    is_valid_ticket = false;
                    sum += v;
                }
            }
            if(is_valid_ticket) valid_tickets.push_back(t);
        }
        return sum;
    }

    bool rule_matches_index(rule_t rule, uint index) {
        auto all_tickets = valid_tickets;
        all_tickets.push_back(ticket);
        for(auto& t : all_tickets) {
            if(!rule_check(rule, t[index])) return false;
        }
        return true;
    }

    void determine_fields() {
        while(rule_indices.size() < rules.size()) {
            for(auto& rule : rules) {
                if(rule_indices.find(rule.first) != rule_indices.end()) continue; // skip rules that are already determined

                vector<uint> potential_indices;
                for(int index = 0; index < ticket.size(); index++) {
                    if(find(mapped_indices.begin(), mapped_indices.end(), index) != mapped_indices.end()) continue; // skip indices that are already determined
                    if(rule_matches_index(rule.second, index)) potential_indices.push_back(index);
                }

                if(potential_indices.size() == 1) {
                    rule_indices[rule.first] = potential_indices.front();
                    mapped_indices.push_back(potential_indices.front());
                    //cout << 100 * rule_indices.size() / rules.size() << "%" << endl;

                    //cout << rule.first << " ";
                    //for(auto& i : potential_indices)
                    //    cout << i << " ";
                    //cout << endl;
                }
            }
        }
    }

    ulong output2() {
        ulong result = 1;
        for(auto& rule : rule_indices) {
            if(rule.first.find("departure") != string::npos) {
                //cout << rule.first << " = " << ticket[rule.second] << endl;
                result *= ticket[rule.second];
            }
        }

        return result;
    }

    private:
    map<string, rule_t> rules;
    map<string, uint> rule_indices;
    vector<uint> mapped_indices;

    ticket_t ticket;
    vector<ticket_t> nearby_tickets;
    vector<ticket_t> valid_tickets;
};

int main(int argc, char** argv) {
    auto t = get_input_all<ticket_decoder>(string(argv[1]));

    cout << t.output1() << endl;
    t.determine_fields();
    cout << t.output2() << endl;

    return 0;
}
