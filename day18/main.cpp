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

struct new_math {
    ulong operate(ulong op1, char op, ulong op2) {
        return (op == '+') ? op1 + op2 : op1 * op2;
    }

    virtual ulong evaluate(string e) {
        uint paren_depth = 0;
        char op = ' ';
        vector<ulong> operands;
        
        for(uint i = 0; i < e.size(); i++) {
            if(e[i] == ' ') continue;

            //cout << e[i];

            if(e[i] == '(') {
                uint paren_begin = i + 1, paren_end;
                paren_depth++; i++;
                while(paren_depth > 0) {
                    if(e[i] == '(') paren_depth++;
                    if(e[i] == ')') paren_depth--;
                    i++;
                }
                paren_end = i - 1;
                //cout << endl;
                ulong paren_eval = evaluate(string(e.begin() + paren_begin, e.begin() + paren_end));
                operands.push_back(paren_eval);
            }

            if(e[i] >= '0' && e[i] <= '9') {
                string operand;
                while(e[i] >= '0' && e[i] <= '9') {
                    operand.push_back(e[i++]);
                }
                operands.push_back(stoul(operand));
            }

            if(e[i] == '+' || e[i] == '*') op = e[i];

            if(operands.size() >= 2) {
                //cout << endl << operands[0] << "," << op << "," << operands[1] << endl;
                ulong op_eval = operate(operands[0], op, operands[1]);
                operands = vector<ulong>{op_eval};
            }
        }
        
        return operands[0];
    }
};

struct new_math_2 : public new_math {
    ulong evaluate(string e) {
        //cout << endl << e << endl;

        uint paren_depth = 0;
        char op = ' ';
        vector<ulong> operands;
        
        for(uint i = 0; i < e.size(); i++) {
            if(e[i] == ' ') continue;

            //cout << e[i];

            if(e[i] == '(') {
                uint paren_begin = i + 1, paren_end;
                paren_depth++; i++;
                while(paren_depth > 0) {
                    if(e[i] == '(') paren_depth++;
                    if(e[i] == ')') paren_depth--;
                    i++;
                }
                paren_end = i - 1;
                //cout << endl;
                ulong paren_eval = evaluate(string(e.begin() + paren_begin, e.begin() + paren_end));
                operands.push_back(paren_eval);
            }

            if(e[i] >= '0' && e[i] <= '9') {
                string operand;
                while(e[i] >= '0' && e[i] <= '9') {
                    operand.push_back(e[i++]);
                }
                operands.push_back(stoul(operand));
            }

            if(e[i] == '*') {
                ulong rhs = evaluate(string(e.begin() + i + 1, e.end()));
                operands.push_back(rhs);
                op = e[i];
                i = e.size() - 1;
            } else if(e[i] == '+') op = e[i];


            if(operands.size() >= 2) {
                //cout << endl << operands[0] << "," << op << "," << operands[1] << endl;
                ulong op_eval = operate(operands[0], op, operands[1]);
                operands = vector<ulong>{op_eval};
            }
        }
        
        return operands[0];
    }
};

int main(int argc, char** argv) {
    auto lines = get_input_perline<string>(string(argv[1]));

    ulong sum1 = 0, sum2 = 0;
    for(auto& line : lines) {
        sum1 += new_math().evaluate(line);
        sum2 += new_math_2().evaluate(line);
    }

    cout << sum1 << endl;
    cout << sum2 << endl;

    return 0;
}
