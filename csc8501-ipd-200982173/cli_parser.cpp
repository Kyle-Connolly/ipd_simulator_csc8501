#include <iostream>
#include <sstream>
#include <algorithm>
#include <unordered_set>
#include "cli_parser.hpp"

CommandOptions CLIParser::parse(int argc, char* argv[]) {
    CommandOptions options{
        0, // --rounds
        0, // --repeats
		{}, // --strategies
		5.0, 3.0, 1.0, 0.0 // --payoff (T,R,P,S) default values
    }; 

    bool epsilonInput = false;
    bool seedInput = false;

    const std::unordered_set<std::string> validStrategies = { "ALLD", "ALLC", "TFT", "GRIM", "PAVLOV", "RND", "CTFT", "PROBER", "TROJAN", "RIVAL" };

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
            std::stringstream strStream(stratlist);
            std::string strategy;

            while (std::getline(strStream, strategy, ',')) {
                // Convert to uppercase
                std::transform(strategy.begin(), strategy.end(), strategy.begin(), ::toupper);

                // Prefix for RND (e.g. RND0.3)
                std::string prefix = strategy.substr(0, 3);

                // Validate against registered strategies
                if (!validStrategies.count(strategy) && prefix != "RND") {
                    throw std::invalid_argument("Error - Invalid strategy: " + strategy);
                }

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
        else if (arg == "--seed" && i + 1 < argc) {
            options.seed = std::stoi(argv[++i]);
            seedInput = true;

            if (options.seed < 0) {
                throw std::invalid_argument("Error - seed must be a non-negative integer");
            }
        }
        else if (arg == "--epsilon" && i + 1 < argc) {
            try {
                options.epsilon = std::stod(argv[++i]);
                if (options.epsilon < 0.0 || options.epsilon > 1.0) {
                    throw std::out_of_range("Error - epsilon must be between 0.0 and 1.0");
                }
                epsilonInput = true;
            }
            catch (const std::exception& e) {
                throw std::invalid_argument(std::string("Error - Invalid value for --epsilon: ") + e.what());
            }
        }
        else if (arg == "--evolve" && i + 1 < argc) {
            int val = std::stoi(argv[++i]);
            if (val != 1) {
                throw std::invalid_argument("Error - --evolve must be 1 to activate evolutionary tournament");
            }
            options.evolve = true;
        }
        else if (arg == "--population" && i + 1 < argc) {
            options.population = std::stoi(argv[++i]);
            if (options.population <= 0) {
                throw std::invalid_argument("Error - --population must be positive");
            }
        }
        else if (arg == "--generations" && i + 1 < argc) {
            options.generations = std::stoi(argv[++i]);
            if (options.generations <= 0) {
                throw std::invalid_argument("Error - --generations must be positive");
            }
        }
        else if (arg == "--scb" && i + 1 < argc) {
            int val = std::stoi(argv[++i]);
            if (val != 1) {
                throw std::invalid_argument("Error - --scb must be 1 for Strategic Complexity Budget");
            }
            options.scb = true;
        }
        else {
            throw std::invalid_argument("Error - Unexpected --argument: " + arg);
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

    for (const auto& strat : options.strategies) {
        std::string name = strat;
        std::transform(name.begin(), name.end(), name.begin(), ::toupper);

        if (name == "TROJAN" && options.rounds < 10) {
            throw std::invalid_argument("Error - TROJAN strategy requires at least 10 rounds.");
        }
        if (name == "PROBER" && options.rounds < 5) {
            throw std::invalid_argument("Error - PROBER strategy requires at least 5 rounds.");
        }
    }

    if (!(options.t > options.r && options.r > options.p && options.p > options.s)) {
        throw std::invalid_argument("Error - payoff inequality detected, requires T > R > P > S to hold");
    }
    if (!(2 * options.r > options.t + options.s)) {
        throw std::invalid_argument("Error - payoff inequality detected, requires 2R > T + S to hold");
    }

    if (epsilonInput != seedInput) {
        throw std::invalid_argument("Error - --epsilon and --seed must be input together.");
    }

    if (epsilonInput && seedInput) {
        options.noiseOn = true; // Enable noise if both seed and epsilon were input
    }

    if (options.evolve) {
        if (options.population <= 0 || options.generations <= 0) {
            throw std::invalid_argument("Error - --evolve requires --population and --generations");
        }
    }

    if (options.scb && !options.evolve) {
        throw std::invalid_argument("Error - --scb can only be used with --evolve.");
    }
    
    return options;
}