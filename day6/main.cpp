#include <iostream>
#include <vector>
#include <fstream>
#include <numeric>
#include <algorithm>

using std::vector;
using std::string;
using std::cout;
using std::endl;

struct group {
    group(vector<string> lines) : people(lines) {
        sort_people();
    }

    string get_set() const {
        return std::accumulate(people.cbegin(), people.cend(), people.front(),
                               [](const string& a, const string& b) {
                                   string result;
                                   std::set_union(a.cbegin(), a.cend(), b.cbegin(), b.cend(), std::back_insert_iterator<string>(result));
                                   return result;
                               });
    }
    string get_intersection() const {
        return std::accumulate(people.cbegin(), people.cend(), people.front(),
                               [](const string& a, const string& b) {
                                   string result;
                                   std::set_intersection(a.cbegin(), a.cend(), b.cbegin(), b.cend(), std::back_insert_iterator<string>(result));
                                   return result;
                               });
    }


    private:
    vector<string> people;

    // sort each person's answers
    void sort_people() {
        for(auto& p : people) 
            sort(p.begin(), p.end());
    }
};

using entry_t = group;
int main(int argc, char** argv) {
    vector<string> lines;
    vector<entry_t> groups;

    std::ifstream input_file{string(argv[1])};
    if(input_file.is_open()) {
        string line;
        while(getline(input_file,line)) {
            if(line.empty()) { 
                groups.push_back(entry_t(lines));
                lines = vector<string>();
            } else {
                lines.push_back(line);
            }
        }
        if(!lines.empty()) { groups.push_back(entry_t(lines)); }
    } else {
        std::cout << "Failed to open input file" << std::endl;
    }

    uint usum = 0, isum = 0;
    for(auto& group : groups) {
        usum += group.get_union().size();
        isum += group.get_intersection().size();
    }

    cout << usum << endl;
    cout << isum << endl;

    return 0;
}
