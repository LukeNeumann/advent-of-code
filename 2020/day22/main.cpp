#include <iostream>
#include <vector>
#include <queue>
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
#include <boost/dynamic_bitset.hpp>

#include "input_handler.hpp"

using std::vector;
using std::queue;
using std::string;
using std::map;
using std::pair;
using std::cout;
using std::endl;

struct combat {
    combat() = default;
    combat(const combat&) = default;
    combat(vector<string> lines) {
        std::smatch m;
        queue<uint> q;
        for(auto& line : lines) {
            if(line.empty())
                players.push_back(q);
            else if(std::regex_match(line, m, std::regex("Player ([0-9]+):")))
                q = queue<uint>();
            else
                q.push(stoi(line));
        }
    }

    vector<queue<uint> >::iterator get_winner() {
        vector<queue<uint> >::iterator result = players.end(); 
        for(auto it = players.begin(); it != players.end(); ++it) {
            if(!it->empty()) {
                if(result == players.end())
                    result = it; // this is the first non-empty player
                else
                    return players.end(); // more than one non-empty player, no winner
            }
        }
        return result;
    }

    void play() {
        while(get_winner() == players.end()) {
            // compare
            vector<uint> draws;
            for(auto& p : players) 
                draws.push_back(p.front());
            uint winner = std::distance(draws.begin(), std::max_element(draws.begin(), draws.end()));

            // push to winner
            players[winner].push(players[winner].front()); // collect own card first
            for(uint i = 0; i < players.size(); ++i) {
                if(i == winner) continue;
                players[winner].push(players[i].front()); // other player cards
            }
            
            // pop all
            for(auto& p : players)
                p.pop();
        }
    }

    bool check_history() {
        for(auto& h : history) {
            //cout << "history: " << endl;
            //print(h);
            if(h == players)
                return true;
        }
        return false;
    }

    static void print(const vector<queue<uint> >& ps) {
        uint i = 0;
        for(const auto& p : ps) {
            cout << i++ << ":";
            auto p_copy = p;
            while(!p_copy.empty()) {
                cout << " " << p_copy.front();
                p_copy.pop();
            }
            cout << endl;
        }
    }

    void play_recursive(uint depth = 0) {
        while(get_winner() == players.end()) {
            // history check
            if(check_history()) {
                for(auto it = players.begin() + 1; it != players.end(); ++it) {
                    while(!it->empty())
                        it->pop(); // clear all players but player 1
                }
                break;
            }
            history.push_back(players);

            // compare
            vector<uint> draws;
            for(auto& p : players) 
                draws.push_back(p.front());
            
            uint winner;
            bool recurse = true;
            for(uint i = 0; i < players.size(); ++i)
                recurse &= draws[i] < players[i].size(); // all players must have as many remaining cards as their draw
            if(recurse) {
                cout << "recursing to determine winner depth=" << depth << endl;
                combat r; // new game
                for(const auto& p : players) {
                    r.players.push_back(queue<uint>());
                    auto p_copy = p;
                    uint num_cards = p_copy.front();
                    p_copy.pop();
                    for(uint i = 0; i < num_cards && !p_copy.empty(); ++i) {
                        r.players.back().push(p_copy.front());
                        p_copy.pop();
                    }
                }
                r.play_recursive(depth + 1);
                winner = std::distance(r.players.begin(), r.get_winner());
            } else {
                winner = std::distance(draws.begin(), std::max_element(draws.begin(), draws.end()));
            }

            // push to winner
            players[winner].push(players[winner].front()); // collect own card first
            for(uint i = 0; i < players.size(); ++i) {
                if(i == winner) continue;
                players[winner].push(players[i].front()); // other player cards
            }
            
            // pop all
            for(auto& p : players)
                p.pop();
        }
    }

    uint score() {
        auto winner = get_winner();
        if(winner == players.end())
            return 0;

        uint result = 0;
        for(uint multiplier = winner->size(); multiplier > 0; --multiplier) {
            result += winner->front() * multiplier;
            winner->pop();
        }

        return result;
    }

    vector<queue<uint> > players;
    vector<vector<queue<uint> > > history;
};

int main(int argc, char** argv) {
    auto lines = get_input_perline<string>(string(argv[1]));

    //combat c(lines);
    //c.play();
    //cout << c.score() << endl;

    combat c2(lines);
    c2.play_recursive();
    combat::print(c2.players);
    cout << c2.score() << endl;

    return 0;
}
