#include <iostream>
#include "cli_parser.hpp"
#include "tournament_manager.hpp"
#include "payoff.hpp"

int main(int argc, char* argv[]) {
    try {
        CommandOptions options = CLIParser::parse(argc, argv);
        Payoff payoff(options.t, options.r, options.p, options.s);
        TournamentManager tournament(options, payoff);

        std::cout << "Running IPD Tournament: " << options.rounds << " rounds | " << options.repeats << " repeats | ";
        if (options.noiseOn) {
            std::cout << "epsilon: " << options.epsilon << " | seed: " << options.seed << "\n";
        }
        else {
            std::cout << "epsilon: 0.0 | seed: 0\n";
        }
        
        tournament.runTournament();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}