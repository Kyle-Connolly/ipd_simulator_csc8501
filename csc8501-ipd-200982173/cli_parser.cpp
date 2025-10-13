#include <iostream>
#include <sstream>
#include <algorithm>
#include "cli_parser.hpp"

CommandOptions CLIParser::parse(int argc, char* argv[]) {
    CommandOptions options{ 0, 
        0, 
        {}, 
        5.0, 
        3.0, 
        1.0, 
        0.0 
    }; 

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--rounds" && i + 1 < argc) {
            options.rounds = std::stoi(argv[++i]);
        }
        else if (arg == "--repeats" && i + 1 < argc) {
            options.repeats = std::stoi(argv[++i]);
        }
        else if (arg == "--strategies" && i + 1 < argc) {
            std::string stratlist = argv[++i];
            std::stringstream ss(stratlist);
            std::string strategy;

            while (std::getline(ss, strategy, ',')) {
                options.strategies.push_back(strategy);
            }
        }
        else if (arg == "--payoff" && i + 1 < argc) {
            std::string payoffList = argv[++i];
            std::stringstream stream(payoffList);
            std::string payoffValue;
            std::vector<double> payoffs;

            while (std::getline(stream, payoffValue, ',')) {
                try {
                    payoffs.push_back(std::stod(payoffValue));
                }
                catch (const std::invalid_argument&) {
                    throw std::invalid_argument("Error - Invalid value for --payoff");
                }
            }

            if (payoffs.size() != 4) {
                throw std::invalid_argument("Error - --payoff must be in the format (T,R,P,S)");
            }
			//replace default payoff values
            options.t = payoffs[0];
            options.r = payoffs[1];
            options.p = payoffs[2];
            options.s = payoffs[3];
        }
        else {
            throw std::invalid_argument("Error - Unexpected argument: " + arg);
        }
    }

    if (options.rounds <= 0) {
        throw std::invalid_argument("Error - non positive integer for rounds.");
    }
        
    if (options.repeats <= 0) {
        throw std::invalid_argument("Error - non positive integer for repeats.");
    }

    if (options.strategies.empty()) {
        throw std::invalid_argument("Error - at least two strategy names required.");
    }
        
    if (options.strategies.size() < 2) {
        throw std::invalid_argument("Error - at least two strategy names required.");
    }

    if (!(options.t > options.r && options.r > options.p && options.p > options.s)) {
        throw std::invalid_argument("Error - payoff inequality detected, requires T > R > P > S to hold");
    }
    if (!(2 * options.r > options.t + options.s)) {
        throw std::invalid_argument("Error - payoff inequality detected, requires 2R > T + S to hold");
    }
        
    return options;
}