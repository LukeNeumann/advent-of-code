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

struct mem_init {
    using cw_t = std::bitset<36>;

    void process_instructions(vector<string> lines) {
        for(auto& line : lines)
            process_instruction(line);
    }

    virtual void set_mask(string mask) {
        ormask = 0;
        andmask = ~0;
        for(uint i = 0; i < mask.size(); ++i) {
            if(mask[i] == '1') ormask.flip(mask.size() - i - 1);
            if(mask[i] == '0') andmask.flip(mask.size() - i - 1);
        }
        //cout << "mask = " << mask << " ormask = " << ormask.to_ulong() << " andmask = " << andmask.to_ulong() << endl;
    }

    virtual void set_mem(ulong addr, ulong val) {
        mem[addr] = (cw_t(val) & andmask | ormask).to_ulong();
        //cout << "mem[" << addr << "] = " << val << endl;
    }

    int process_instruction(string inst) {
        std::smatch m;
        if(std::regex_match(inst, m, std::regex("mask = ([X01]{36})"))){
            set_mask(m[1]);
        } else if(std::regex_match(inst, m, std::regex("mem.([0-9]{1,36}). = ([0-9]{1,36})"))) {
            set_mem(stoul(m[1]), stoul(m[2]));
        } else {
            return 1;
        }

        return 0;
    }

    ulong sum_mem() {
        ulong sum = 0;
        for(const auto& m : mem) 
            sum += m.second;
        return sum;
    }

    private:
    cw_t andmask, ormask;

    protected:
    map<ulong, ulong> mem;
};

struct mem_init_v2 : public mem_init {
    virtual void set_mask(string mask) {
        this->mask = mask;
        //cout << "mask = " << mask << endl;
    }

    virtual void set_mem(ulong addr, ulong val) {
        cw_t addr_bits(addr);
        vector<uint> floating_bits;

        //first pass to set all handle all non-floating mask bits and save floating mask bits away for a second pass
        for(uint i = 0; i < mask.size(); ++i) {
            char sb_mask = mask[mask.size() - i - 1]; // single-bit mask 
            if(sb_mask == '1') addr_bits[i] = 1;
            if(sb_mask == 'X') {
                addr_bits[i] = 0;
                floating_bits.push_back(i);
            }
        }

        //second pass: iterate through all possible combinations of floating bits
        for(uint float_enable = 0; float_enable < std::pow(2, floating_bits.size()); float_enable++) {
            //cout << "float_enable = " << float_enable << " ";
            auto addr_bits_copy = addr_bits;
            for(uint i = 0; i < floating_bits.size(); ++i) {
                if((float_enable & (1<<i)) != 0) {
                    addr_bits_copy.flip(floating_bits[i]);
                    //cout << "floating_bit " << floating_bits[i] << " ";
                }
            }
            //cout << endl;

            //cout << "mem[" << addr_bits_copy.to_ulong()<< "] = " << val << endl;
            mem[addr_bits_copy.to_ulong()] = val;
        }
        //cout << "mem[" << addr << "] = " << val << endl;
    }

    private:
    string mask;
};

int main(int argc, char** argv) {
    auto lines = get_input_perline<string>(string(argv[1]));

    mem_init m1;
    mem_init_v2 m2;

    m1.process_instructions(lines);
    m2.process_instructions(lines);

    cout << m1.sum_mem() << endl;
    cout << m2.sum_mem() << endl;

    return 0;
}
