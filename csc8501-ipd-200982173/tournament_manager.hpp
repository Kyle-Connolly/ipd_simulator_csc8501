#pragma once
#include <string>
#include <vector>
#include "cli_parser.hpp"
#include "payoff.hpp"

struct MatchStatistics {
    double p1Mean;
    double p2Mean;
    double p1Stdev;
    double p2Stdev;
    std::string p1CiLowerStr;
    std::string p1CiUpperStr;
    std::string p2CiLowerStr;
    std::string p2CiUpperStr;
};

class TournamentManager {
public:
    TournamentManager(const CommandOptions& options, const Payoff& payoff);
    void runTournament();

private:
    CommandOptions options;
    Payoff payoff;

    std::string createFilename() const;
    std::pair<std::vector<double>, std::vector<double>> runMatchup(const std::string& strat1, const std::string& strat2);
    MatchStatistics calculateStatistics(const std::vector<double>& p1Scores, const std::vector<double>& p2Scores);
    void writeResults(std::ofstream& csv, const std::string& strat1, const std::string& strat2, const MatchStatistics& stats);
};