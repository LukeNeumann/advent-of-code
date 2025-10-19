#include <iostream>
#include <vector>
#include <fstream>
#include <numeric>
#include <algorithm>
#include <map>
#include <iterator>
#include <functional>
#include <cmath>
#include <boost/range/algorithm/count.hpp>

#include "input_handler.hpp"

using std::vector;
using std::string;
using std::map;
using std::pair;
using std::cout;
using std::endl;

struct bus_info {
    bus_info(ulong a) : id(a), idx(0) {}
    bus_info(ulong a, ulong b) : id(a), idx(b) {}
    ulong id;
    uint idx;
};

struct bus_notes {
    bus_notes(vector<string> args) {
        departure = stoi(args[0]);

        uint i = 0;
        auto prev_sep = args[1].begin();
        auto sep = find(args[1].begin(), args[1].end(), ','); 
        while(sep != args[1].end()) {
            try { 
                buses.push_back(bus_info(stoi(string(prev_sep, sep)), i));
            } catch (std::exception e) {}
            prev_sep = sep + 1;
            sep = find(prev_sep, args[1].end(), ','); 
            ++i;
        }
        try { 
            buses.push_back(bus_info(stoi(string(prev_sep, sep)), i));
        } catch (std::exception e) {}
    }

    uint find_bus() {
        for(uint d = departure; true; ++d) {
            for(const auto& bus : buses) {
                if(d % bus.id == 0) return bus.id;
            }
        }

        return 0;
    }

    uint output1() {
        uint bus = find_bus();
        return (bus - (departure % bus)) * bus;
    }

    void print(vector<bus_info> b) {
        for(const auto& bus : b) {
            cout << "("<<bus.id << ", " << bus.idx << "), ";
        }
        cout << endl;
    }

    bool is_gold_coin_winner(ulong d, const vector<bus_info> & b) {
        for(auto& bus : b) {
            if((d + bus.idx) % bus.id != 0) return false;
        }

        return true;
    }

    ulong output2() {
        uint i = 0;
        vector<bus_info> figured_buses;
        figured_buses.push_back(buses[i]);
        ulong inc = buses[i].id;
        ulong init = buses[i].id - buses[i].idx;

        for(ulong d = init; true; d += inc){
            if(is_gold_coin_winner(d, figured_buses)) {
                // increment by the lcm of all the figured buses
                inc = std::accumulate(figured_buses.begin(), figured_buses.end(), figured_buses.front(), 
                      [](bus_info a, bus_info b) { return std::lcm(a.id, b.id); }).id; 

                if(++i >= buses.size()) return d; //we've figured out all the buses

                figured_buses.push_back(buses[i]); // add the next bus to figure out
            }
        }

        return 0;
    }

    private:
    uint departure;
    vector<bus_info> buses;
};

int main(int argc, char** argv) {
    auto n = get_input_all<bus_notes>(string(argv[1]));

    cout << n.output1() << endl;
    cout << n.output2() << endl;

    return 0;
}
