#pragma once
#include <string>
#include <vector>
#include "cli_parser.hpp"
#include "payoff.hpp"

class TournamentManager {
public:
    TournamentManager(const CommandOptions& options, const Payoff& payoff);

    void runTournament();

private:
    CommandOptions options;
    Payoff payoff;
    std::string createFilename() const;
};