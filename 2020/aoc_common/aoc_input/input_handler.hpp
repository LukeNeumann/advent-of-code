#pragma once

#include <iostream>
#include <vector>
#include <fstream>

template <class T>
std::vector<T> get_input_perline(std::string filename) {
    std::vector<T> entries;
    std::ifstream input_file{filename};
    if(input_file.is_open()) {
        std::string line;
        while(getline(input_file,line)) {
            entries.push_back(T(line));
        }
    } else {
        std::cout << "Failed to open input file" << std::endl;
    }
    return entries;
}

template <class T>
T get_input_all(std::string filename) {
    std::vector<std::string> lines;
    std::ifstream input_file{filename};
    if(input_file.is_open()) {
        std::string line;
        while(getline(input_file,line)) {
            lines.push_back(line);
        }
    } else {
        std::cout << "Failed to open input file" << std::endl;
    }
    return T(lines);
}


std::vector<std::string> split(std::string s, char separator) {
    std::vector<std::string> result;
    auto prev_sep = s.begin();
    auto sep = find(s.begin(), s.end(), separator);
    while(sep != s.end()) {
        result.push_back(std::string(prev_sep,sep));
        prev_sep = sep + 1;
        sep = find(prev_sep, s.end(), separator);
    }
    result.push_back(std::string(prev_sep,sep));

    return result;
}

std::string strip(std::string s, char separator) {
    std::string::iterator it1, it2;
    for(it1 = s.begin(); (*it1) == separator; it1++) {}
    for(it2 = s.end() - 1; (*it2) == separator; it2--) {}
    return std::string(it1, it2+1);
}
