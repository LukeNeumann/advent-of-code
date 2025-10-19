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

struct instruction {
    instruction(string s) {
        std::smatch m;
        std::regex_match(s, m, std::regex("([^ ]+) ([+-][0-9]+)"));
        op = m[1];
        arg = std::stoi(m[2]);
    }

    operator string() {
        return op + ", " + std::to_string(arg); 
    }

    string op;
    int arg;
};

struct simulator {
    void execute(instruction i) {
        history[pc]++;
        // cout << "Executing " << (string)i << endl;

        if(i.op == "acc") accumulator += i.arg;
        if(i.op == "jmp") pc += i.arg;
        else pc++;
    }

    int execute(vector<instruction> insts) {
        pc = 0;
        accumulator = 0;
        history = vector<uint>(insts.size(), 0);
        while(pc >= 0 &&  pc < insts.size()) {
            if(history[pc] != 0) return -1;
            execute(insts[pc]);
        }
        
        return 0;
    }

    uint accumulator;
    uint pc; // program counter
    vector<uint> history; // num executions for each line
};

using entry_t = instruction;
int main(int argc, char** argv) {
    vector<entry_t> entries;

    std::ifstream input_file{string(argv[1])};
    if(input_file.is_open()) {
        string line;
        while(getline(input_file,line)) {
            entries.push_back(entry_t(line));
        }
    } else {
        std::cout << "Failed to open input file" << std::endl;
    }

    simulator s;
    s.execute(entries);

    cout << s.accumulator << endl;

    for(uint i = 0; i < entries.size(); i++) {
        vector<entry_t> fixed_entries;
        if(entries[i].op == "nop") {
            fixed_entries = entries;
            fixed_entries[i].op = "jmp";
        } else if(entries[i].op == "jmp") {
            fixed_entries = entries;
            fixed_entries[i].op = "nop";
        } else {
            continue;
        }

        if(!s.execute(fixed_entries)) {
            cout << s.accumulator << endl;
        }
    }

    return 0;
}
