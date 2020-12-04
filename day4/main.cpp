#include <iostream>
#include <vector>
#include <fstream>
#include <regex>

using std::stoi;

struct passport {
    std::map<std::string, std::string> fields;

    void add_field(std::string key_value) {
        std::smatch match;
        std::regex_search(key_value, match, std::regex("([^: ]+):([^: ]+)"));
        fields[match[1]] = match[2];
    }

    passport(std::vector<std::string> lines) {
        for(auto& line : lines) {
            auto prev_sep = line.begin(), sep = std::find(line.begin(), line.end(), ' ');
            add_field(std::string(prev_sep, sep));
            while(sep != line.end()) {
                prev_sep = sep;
                sep = std::find(sep + 1, line.end(), ' ');
                add_field(std::string(prev_sep + 1, sep));
            }
        }
    }

    void print() const {
        for(auto it = fields.cbegin(); it != fields.cend(); ++it) {
            std::cout << it->first << ":" << it->second << " ";
        }
        std::cout << std::endl;
    }


    std::vector<std::string> req_fields{"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"};
};

bool valid1(const passport& p) {
    for(auto it = p.req_fields.cbegin(); it != p.req_fields.cend(); ++it) {
        if(p.fields.find(*it) == p.fields.end()) {
            // std::cout << "Invalid! Missing " << *it << std::endl;
            return false;
        }
    }
    return true;
}

bool valid2(const passport& p) {
    for(auto it = p.req_fields.cbegin(); it != p.req_fields.cend(); ++it) {
        if(p.fields.find(*it) == p.fields.end()) {
            // std::cout << "Invalid! Missing " << *it << std::endl;
            return false;
        }

        bool field_checks_passed = true;
        const std::string field = *it;
        const std::string value = p.fields.at(*it);
        try {
            if(field == "byr" && (stoi(value) < 1920 || stoi(value) > 2002)) field_checks_passed = false; 
            if(field == "iyr" && (stoi(value) < 2010 || stoi(value) > 2020)) field_checks_passed = false;
            if(field == "eyr" && (stoi(value) < 2020 || stoi(value) > 2030)) field_checks_passed = false;
            if(field == "hgt") {
                std::smatch m;
                if(std::regex_match(value, m, std::regex("([0-9]+)(cm|in)"))) {
                    if(m[2] != "cm" && m[2] != "in") {
                        //std::cout << "invalid height unit " << m[2] << std::endl;
                        return false;
                    } else if(stoi(m[1]) < (m[2] == "cm" ? 150 : 59) || stoi(m[1]) > (m[2] == "cm" ? 193 : 76)) {
                        //std::cout << "invalid height measurement " << value << std::endl;
                        return false;
                    }
                } else { return false; }
            }
            if(field == "hcl") {
                if(!std::regex_match(value, std::regex("#[0-9a-f]{6}"))) field_checks_passed = false;
            }
            if(field == "ecl") {
                std::vector<std::string> eye_colors{"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};
                if(std::find(eye_colors.begin(), eye_colors.end(), value) == eye_colors.end()) field_checks_passed = false;
            }
            if(field == "pid") {
                if(!std::regex_match(value, std::regex("[0-9]{9}"))) field_checks_passed = false;
            }
        } catch(std::exception e) {
            field_checks_passed = false;
        }

        if(!field_checks_passed) {
            //std::cout << "invalid " << field << " " << value << std::endl;
            return false;
        }

    }
    return true;
}

uint count_valid_passports(std::vector<passport> passports, std::function<bool(const passport&)> policy) {
    uint valid_count = 0;
    for(std::vector<passport>::const_iterator it = passports.cbegin(); it != passports.cend(); ++it) {
        //it->print();
        valid_count += policy(*it);
    }
    return valid_count;
}

int main(int argc, char** argv) {
    std::vector<std::string> lines;
    std::vector<passport> passports;

    std::ifstream input_file{std::string(argv[1])};
    if(input_file.is_open()) {
        std::string line;
        while(getline(input_file,line)) {
            if(line.empty()) { 
                passports.push_back(passport(lines));
                lines = std::vector<std::string>();
            } else {
                lines.push_back(line);
            }
        }
        if(!lines.empty()) { passports.push_back(passport(lines)); }
    } else {
        std::cout << "Failed to open input file" << std::endl;
    }

    std::cout << count_valid_passports(passports, valid1) << " valid passports" << std::endl;
    std::cout << count_valid_passports(passports, valid2) << " valid passports" << std::endl;

    return 0;
}
