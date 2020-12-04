#include <iostream>
#include <vector>
#include <fstream>
#include <regex>

bool policy1(uint min, uint max, std::string letter, std::string password) {
    uint letter_count = 0;
    auto found = std::find(password.begin(), password.end(), letter[0]);
    while(found != password.end()) {
        found = std::find(found + 1, password.end(), letter[0]);
        letter_count++;
    }

    return min <= letter_count && letter_count <= max;
}

bool policy2(uint min, uint max, std::string letter, std::string password) {
    return (password[min - 1] == letter[0]) ^ (password[max - 1] == letter[0]);
}

uint count_valid_passwords(std::vector<std::string> lines, std::function<bool(uint, uint, std::string, std::string)> policy) {
    uint valid_count = 0;
    for(std::vector<std::string>::const_iterator line = lines.cbegin(); line != lines.cend(); line++) {
        std::smatch match;
        if(std::regex_match(*line, match, std::regex("^([0-9]+)-([0-9]+) ([a-z]): ([a-z]+)$"))) {
            valid_count += policy(stoi(match[1]), stoi(match[2]), match[3], match[4]);
        } else {
            std::cout << "Invalid password line \"" << *line << "\"" << std::endl;
        }
    }
    return valid_count;
}

int main(int argc, char** argv) {
    std::vector<std::string> lines;

    std::ifstream input_file{std::string(argv[1])};
    if(input_file.is_open()) {
        std::string line;
        while(getline(input_file,line)) {
            lines.push_back(line);
        }
    } else {
        std::cout << "Failed to open input file" << std::endl;
    }

    std::cout << count_valid_passwords(lines, policy1) << " valid passwords" << std::endl;
    std::cout << count_valid_passwords(lines, policy2) << " valid passwords" << std::endl;

    return 0;
}
