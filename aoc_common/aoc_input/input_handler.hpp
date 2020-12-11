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
