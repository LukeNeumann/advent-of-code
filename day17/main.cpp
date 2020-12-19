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


template<uint D, class T>
struct dvec : public vector<dvec<D-1,T> > {
    dvec(int n = 0, const T& val = T()) : vector<dvec<D-1,T> >(n, dvec<D-1,T>(n, val)) {}
    dvec(vector<string> lines) {
        if constexpr(D > 2) {
            this->push_back(dvec<D-1,T>(lines));
        } else {
            //initialize the inner 2 dimensions with lines
            vector<dvec<1,T> >(0, dvec<1,T>());
            for(auto& line : lines) {
                dvec<1,T> new_row{};
                for(auto& c : line) {
                    new_row.push_back(c == '#');
                }
                this->push_back(new_row);
            }
        }
    }

    void set_all(const T& val = T()) {
        for(auto& e : *this) e.set_all(val);
    }

    void increase_size() {
        for(auto& e : *this) e.increase_size();
        dvec<D-1,T> d = this->front();
        d.set_all(); // reset values for new sections
        this->insert(this->begin(),d);
        this->insert(this->end(),d);
    }

    uint count() {
        uint result = 0;
        for(auto& e : *this) result += e.count();
        //cout << D << " " << this->size() << " " << result << endl;
        return result;
    }
};

template<class T>
struct dvec<1,T> : public vector<T> {
    dvec(int n = 0, const T& val = T()) : vector<T>(n,val) {}

    void set_all(const T& val = T()) {
        for(uint i = 0; i < this->size(); i++) (*this)[i] = val;
    }

    void increase_size() {
        this->insert(this->begin(),T());
        this->insert(this->end(),T());
    }

    uint count() {
        uint result = std::accumulate(this->begin(), this->end(), 0);
        //cout << 1 << " " << this->size() << " " << result << endl;
        return result;
    }
};

template<uint D, class T>
uint count_adjacent(dvec<D,T>& d, vector<uint> indices) {
    uint result = 0;
    uint index = indices.front(); indices.erase(indices.begin());
    for(uint i = ((index==0) ? 0 : index - 1); i <= ((index==d.size()-1) ? index : index + 1); i++) {
        //cout << i << " ";
        result += count_adjacent<D-1,T>(d[i], indices);
    }
    return result;
}
template<>
uint count_adjacent<1,bool>(dvec<1,bool>& d, vector<uint> indices) {
    uint result = 0;
    uint index = indices.front(); indices.erase(indices.begin());
    for(uint i = ((index==0) ? 0 : index - 1); i <= ((index==d.size()-1) ? index : index + 1); i++) {
        result += d[i];
        //cout << i << ": " << d[i] << endl;
    }
    return result;
}

template<uint D, uint P, class T>
void _cycle(dvec<P,T>& prev_d, dvec<D,T>& d, vector<uint> indices = {}) {
    for(uint i = 0; i < d.size(); i++) {
        auto new_indices = indices;
        new_indices.push_back(i);
        if constexpr(D > 1) {
            _cycle<D-1,P,T>(prev_d, d[i], new_indices);
        } else {
            uint active_neighbors = count_adjacent(prev_d, new_indices) - d[i];
            //for(auto& ind : new_indices) cout << ind << " ";
            //cout << "neighbors=" << active_neighbors << " active=" << d[i] << endl;

            if(d[i] == true) {
                d[i] = active_neighbors == 2 || active_neighbors == 3;
            } else {
                d[i] = active_neighbors == 3;
            }
        }
    }
}

template<uint D, class T>
void cycle(dvec<D,T>& d, uint count) {
    for(uint i = 0; i < count; i++) {
        d.increase_size();
        auto prev_d = d;
        _cycle<D,D,T>(prev_d, d);
    }
}

int main(int argc, char** argv) {
    auto lines = get_input_perline<string>(string(argv[1]));

    dvec<3,bool> d3(lines);
    dvec<4,bool> d4(lines);

    cycle<3,bool>(d3, 6);
    cout << d3.count() << endl;
    cycle<4,bool>(d4, 6);
    cout << d4.count() << endl;

    return 0;
}
