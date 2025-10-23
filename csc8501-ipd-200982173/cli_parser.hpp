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
	bool noiseOn = false; // Flag to indicate if implementation noise is enabled for strategy actions
    int seed = 0; // Default seed
	double epsilon = 0.0; // Probability of action flip, default = no noise
    bool evolve = false;
    int population = 0;
    int generations = 0;
    bool scb = false; // Strategic Complexity Budget (SCB)
    std::string format;
};

class CLIParser {
public:
    static CommandOptions parse(int argc, char* argv[]);
};