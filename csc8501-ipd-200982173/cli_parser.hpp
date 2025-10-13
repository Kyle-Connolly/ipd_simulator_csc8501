#pragma once
#include <string>
#include <vector>
#include <stdexcept>

struct CommandOptions {
    int rounds;
    int repeats;
    std::vector<std::string> strategies;
    double t;
    double r;
    double p;
    double s;
};

class CLIParser {
public:
    static CommandOptions parse(int argc, char* argv[]);
};