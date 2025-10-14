#pragma once
#include <string>
#include <vector>
#include <map>
#include "cli_parser.hpp"
#include "payoff.hpp"

struct MatchStatistics {
    double p1Mean;
    double p2Mean;
    double p1Stdev;
    double p2Stdev;
    std::string p1CILower;
    std::string p1CIUpper;
    std::string p2CILower;
    std::string p2CIUpper;
};

class TournamentManager {
public:
    TournamentManager(const CommandOptions& options, const Payoff& payoff);
    void runTournament();

private:
    CommandOptions options;
    Payoff payoff;

    std::string createFilename() const;
    std::ofstream openResultsFile(std::string& outFilename) const;
    void writeResultsFileHeader(std::ofstream& csv) const;
    void writePayoffMatrixFile(const std::vector<std::string>& strategies, const std::map<std::pair<std::string, std::string>, MatchStatistics>& results) const;
    std::pair<std::vector<double>, std::vector<double>> runIPD(const std::string& strat1, const std::string& strat2);
    MatchStatistics calculateStatistics(const std::vector<double>& p1Scores, const std::vector<double>& p2Scores);
    void writeResults(std::ofstream& csv, const std::string& strat1, const std::string& strat2, const MatchStatistics& stats);
};