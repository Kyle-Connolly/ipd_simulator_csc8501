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

template <typename T>
class TournamentManager {
public:
    TournamentManager(const CommandOptions& options, const Payoff<T>& payoff);
    void runTournament();
    //void runEvolutionaryTournament();
private:
    const CommandOptions& options;
    const Payoff<T>& payoff;

    std::pair<std::vector<double>, std::vector<double>> runIPD(const std::string& strat1, const std::string& strat2);
    std::string createFilename(const std::string& prefix, const std::string& extension = ".csv") const;
    std::ofstream openPairwisePayoffsFile(std::string& outFilename) const;
    
    MatchStatistics calculateStatistics(const std::vector<double>& p1Scores, const std::vector<double>& p2Scores) const;

    void outputPairwisePayoffsStats(const std::string& strat1, const std::string& strat2, const MatchStatistics& stats) const;
    void writePairwisePayoffsFile(const std::map<std::pair<std::string, std::string>, MatchStatistics>& allResults) const;
    void writePayoffMatrixFile(const std::vector<std::string>& strategies, const std::map<std::pair<std::string, std::string>, MatchStatistics>& results) const;
    void writeLeaderboardFile(const std::vector<std::string>& strategies, const std::map<std::pair<std::string, std::string>, MatchStatistics>& results) const;
};

#include "tournament_manager.tpp"