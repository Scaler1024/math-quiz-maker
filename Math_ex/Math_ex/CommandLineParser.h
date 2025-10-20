#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <functional>

#include <unordered_map>


class CommandLineParser {
private:
    unordered_map<string, string> arguments;

public:
    void parse(int argc, char* argv[]) {
        for (int i = 1; i < argc; i++) {
            string arg = argv[i];

            if (arg == "-n" && i + 1 < argc) {
                arguments["-n"] = argv[++i];
            }
            else if (arg == "-r" && i + 1 < argc) {
                arguments["-r"] = argv[++i];
            }
            else if (arg == "-e" && i + 1 < argc) {
                arguments["-e"] = argv[++i];
            }
            else if (arg == "-a" && i + 1 < argc) {
                arguments["-a"] = argv[++i];
            }
        }
    }

    bool has(const string& key) const {
        return arguments.find(key) != arguments.end();
    }

    string get(const string& key, const string& defaultValue = "") const {
        auto it = arguments.find(key);
        if (it != arguments.end()) {
            return it->second;
        }
        return defaultValue;
    }
};
