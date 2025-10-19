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

struct ship_state {
    ship_state() {
        facing = 0;
        position = pair<int, int>(0, 0);
    }

    void process_instructions(vector<string> instructions) {
        for(auto& inst : instructions) process_instruction(inst);
    }

    virtual void process_instruction(string instruction) {
        int amt = stoi(string(instruction.begin() + 1, instruction.end()));
        char cmd = instruction.front();

        if(cmd == 'N') position.second += amt;
        if(cmd == 'S') position.second -= amt;
        if(cmd == 'E') position.first += amt;
        if(cmd == 'W') position.first -= amt;

        if(cmd == 'L') facing += amt;
        if(cmd == 'R') facing -= amt;
        facing = facing % 360;

        if(cmd == 'F') {
            position.first += amt*std::cos(facing * M_PI / 180);
            position.second += amt*std::sin(facing * M_PI / 180);
        }
    }

    uint manhattan_distance() {
        return std::abs(position.first) + std::abs(position.second);
    }
    
    protected:
    int facing;
    pair<int, int> position;
};

struct waypoint_state : public ship_state {
    waypoint_state() {
        waypoint = pair<int, int>(10, 1);
    }

    virtual void process_instruction(string instruction) {
        int amt = stoi(string(instruction.begin() + 1, instruction.end()));
        char cmd = instruction.front();

        if(cmd == 'N') waypoint.second += amt;
        if(cmd == 'S') waypoint.second -= amt;
        if(cmd == 'E') waypoint.first += amt;
        if(cmd == 'W') waypoint.first -= amt;

        if(cmd == 'L' || cmd == 'R'){
            float angle = atan((float)waypoint.second / (float)waypoint.first) + (waypoint.first < 0 ? M_PI : 0);
            float magnitude = sqrt(pow(waypoint.second,2) + pow(waypoint.first, 2));
            angle += (cmd == 'L' ? 1 : -1) * amt * M_PI / 180;

            waypoint.first = round(magnitude * std::cos(angle));
            waypoint.second = round(magnitude * std::sin(angle));
        }

        if(cmd == 'F') {
            position.first += amt * waypoint.first;
            position.second += amt * waypoint.second;
        }
    }

    pair<int, int> waypoint;
};

int main(int argc, char** argv) {
    auto instructions = get_input_perline<string>(string(argv[1]));

    ship_state s1;
    s1.process_instructions(instructions);
    cout << s1.manhattan_distance() << endl;

    waypoint_state s2;
    s2.process_instructions(instructions);
    cout << s2.manhattan_distance() << endl;

    return 0;
}
