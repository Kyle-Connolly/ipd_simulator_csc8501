#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <map>
#include <algorithm>
#include <numeric>
#include "game_manager.hpp"
#include "strategy_creator.hpp"

template <typename T>
TournamentManager<T>::TournamentManager(const CommandOptions& options, const Payoff<T>& payoff)
    : options(options), payoff(payoff) {
}

template <typename T>
MatchStatistics TournamentManager<T>::calculateStatistics(const std::vector<double>& p1Scores, const std::vector<double>& p2Scores) const {
    MatchStatistics stats;

    // Calculate mean - lambda function
    auto calculateMean = [](const std::vector<double>& scores) -> double {
        if (scores.empty()) {
            return 0.0;
        }
        double sum = 0.0;
        for (double s : scores) {
            sum += s;
        }
        return sum / scores.size();
    };

    // Calculate standard deviation - lambda function
    auto calculateStdev = [](const std::vector<double>& scores, double mean) -> double {
        if (scores.size() <= 1) {
            return 0.0;
        }
        double sumSquared = 0.0;
        for (double s : scores) {
            sumSquared += (s - mean) * (s - mean);
        }
        return std::sqrt(sumSquared / (scores.size() - 1));
    };

    stats.p1Mean = calculateMean(p1Scores);
    stats.p2Mean = calculateMean(p2Scores);
    stats.p1Stdev = calculateStdev(p1Scores, stats.p1Mean);
    stats.p2Stdev = calculateStdev(p2Scores, stats.p2Mean);

    // Calculate Confidence Interval 
    if (options.repeats > 1) {
        double p1StandardError = stats.p1Stdev / std::sqrt(options.repeats);
        double p2StandardError = stats.p2Stdev / std::sqrt(options.repeats);

        double p1CiRange = 1.96 * p1StandardError;
        double p2CiRange = 1.96 * p2StandardError;

        stats.p1CILower = std::to_string(stats.p1Mean - p1CiRange);
        stats.p1CIUpper = std::to_string(stats.p1Mean + p1CiRange);
        stats.p2CILower = std::to_string(stats.p2Mean - p2CiRange);
        stats.p2CIUpper = std::to_string(stats.p2Mean + p2CiRange);
    }
    else {
        stats.p1CILower = "N/A";
        stats.p1CIUpper = "N/A";
        stats.p2CILower = "N/A";
        stats.p2CIUpper = "N/A";
    }

    return stats;
}

template <typename T>
std::string TournamentManager<T>::createFilename(const std::string& prefix, const std::string& extension) const {
    auto systemTime = std::chrono::system_clock::now();
    std::time_t systemTimeConversion = std::chrono::system_clock::to_time_t(systemTime);
    std::tm dateTime{};

#ifdef _WIN32
    localtime_s(&dateTime, &systemTimeConversion);
#else
    localtime_r(&systemTimeConversion, &dateTime);
#endif

    std::ostringstream filename;
    filename << prefix << "_" << std::put_time(&dateTime, "%Y%m%d_%H%M%S") << extension;
    return filename.str();
}

template <typename T>
std::ofstream TournamentManager<T>::openPairwisePayoffsFile(std::string& outFilename) const {
    outFilename = createFilename("pairwise payoffs");
    std::ofstream csv(outFilename);

    if (!csv.is_open()) {
        throw std::runtime_error("Results file " + outFilename + " was not created");
    }

    return csv;
}

template <typename T>
void TournamentManager<T>::outputPairwisePayoffsStats(const std::string& strat1, const std::string& strat2, const MatchStatistics& stats) const {
	// Console output
	std::cout << "\n---------CONFIDENCE INTERVAL---------";
    std::cout << "\n" << strat1 << " vs " << strat2 << ":\n";
    std::cout << " Player 1 (" << strat1 << ") Mean = " << stats.p1Mean << ", 95% CI [" << stats.p1CILower << ", " << stats.p1CIUpper << "]\n";
    std::cout << " Player 2 (" << strat2 << ") Mean = " << stats.p2Mean << ", 95% CI [" << stats.p2CILower << ", " << stats.p2CIUpper << "]\n";
    std::cout << "==========================================================================================\n";
}

template <typename T>
void TournamentManager<T>::writePairwisePayoffsFile(const std::map<std::pair<std::string, std::string>, MatchStatistics>& allResults) const {
    std::string filename = createFilename("pairwise_payoffs");
    
    std::ofstream csv(filename);
    if (!csv.is_open()) {
        throw std::runtime_error("Pairwise results file " + filename + " could not be created");
    }

    csv << "Rounds: " << options.rounds << "\n";
    csv << "Repetitions: " << options.repeats << "\n";
    csv << "Payoff: " << payoff.getT() << "," << payoff.getR() << "," << payoff.getP() << "," << payoff.getS() << "\n\n\n";
    csv << "Strategy[1],Strategy[2],Mean[1],Mean[2],Stdev[1],Stdev[2],CI_Low[1],CI_Up[1],CI_Low[2],CI_Up[2]\n";

    for (const auto& [pair, stats] : allResults) {
        const std::string& strat1 = pair.first;
        const std::string& strat2 = pair.second;
        
        // CSV output
        csv << strat1 << "," << strat2 << ","
            << stats.p1Mean << "," << stats.p2Mean << ","
            << stats.p1Stdev << "," << stats.p2Stdev << ","
            << stats.p1CILower << "," << stats.p1CIUpper << ","
            << stats.p2CILower << "," << stats.p2CIUpper << "\n";
    }

    csv.close();
    std::cout << "\n- Pairwise payoffs results saved in: " << filename;
}

template <typename T>
void TournamentManager<T>::writePayoffMatrixFile(const std::vector<std::string>& strategies, const std::map<std::pair<std::string, std::string>, MatchStatistics>& results) const {
    std::string filename = createFilename("payoff_matrix");
    std::ofstream payoffMatrixFile(filename);

    if (!payoffMatrixFile.is_open()) {
        throw std::runtime_error("Error - payoff matrix file: " + filename + "could not be created");
    }

    // Headers/lables/top row
    payoffMatrixFile << ",";
    for (const auto& strat : strategies) {
        payoffMatrixFile << strat << ",";
    }
    payoffMatrixFile << "\n";

    for (const auto& rowStrat : strategies) {
        payoffMatrixFile << rowStrat << ",";

        for (const auto& columnStrat : strategies) {
			// Strategies do not play against themselves so mark with "-"
            if (rowStrat == columnStrat) {
                payoffMatrixFile << "-,";
            }
            else {
                auto mapIterator = results.find({ rowStrat, columnStrat });
                
                if (mapIterator != results.end()) {
                    payoffMatrixFile << mapIterator->second.p1Mean << ",";
                }
                else {
                    payoffMatrixFile << "N/A,";
                }
            }
        }
        payoffMatrixFile << "\n";
    }

    payoffMatrixFile.close();

    std::cout << "\n- Payoff matrix saved in: " << filename;
}

template <typename T>
void TournamentManager<T>::writeLeaderboardFile(const std::vector<std::string>& strategies, const std::map<std::pair<std::string, std::string>, MatchStatistics>& results) const {
    std::string filename = createFilename("leaderboard");
    std::ofstream csv(filename);

    if (!csv.is_open()) {
        throw std::runtime_error("Leaderboard file " + filename + " could not be created");
    }

    std::map<std::string, std::vector<double>> strategyScores;

    for (const auto& [pair, stats] : results) {
        const std::string& strat1 = pair.first;
        const std::string& strat2 = pair.second;

        strategyScores[strat1].push_back(stats.p1Mean);
        strategyScores[strat2].push_back(stats.p2Mean);
    }

    struct LeaderboardEntry {
        std::string name;
        double mean;
        double stdev;
    };

    std::vector<LeaderboardEntry> leaderboard;

    for (const auto& [strat, scores] : strategyScores) {
        MatchStatistics stats = calculateStatistics(scores, {});
        leaderboard.push_back({ strat, stats.p1Mean, stats.p1Stdev });
    }

    // Sort leaderboard by performance (descending order of mean payoff)
    std::sort(leaderboard.begin(), leaderboard.end(), [](const auto& a, const auto& b) { return a.mean > b.mean; });

    csv << "Rank,Strategy,Mean,Stdev\n";

    int leaderboardRank = 1;
    for (const auto& entry : leaderboard) {
        csv << leaderboardRank++ << "," << entry.name << "," << entry.mean << "," << entry.stdev << "\n";
    }

    csv.close();
    std::cout << "\n- Leaderboard saved in: " << filename;
}

template <typename T>
std::pair<std::vector<double>, std::vector<double>> TournamentManager<T>::runIPD(const std::string& strat1, const std::string& strat2) {
    std::vector<double> p1Scores;
    std::vector<double> p2Scores;

    // Single random number generator for the whole tournament
    std::mt19937 randNumGen(options.seed);

    for (int r = 0; r < options.repeats; ++r) {
        auto p1Strategy = StrategyCreator::createStrategy(strat1);
        auto p2Strategy = StrategyCreator::createStrategy(strat2);

        GameManager<double> game(std::move(p1Strategy), std::move(p2Strategy), payoff, options.epsilon, randNumGen, options.noiseOn);
        game.runGame(options.rounds, r + 1, options.repeats);

        p1Scores.push_back(game.getPlayer1Strategy()->getScore());
        p2Scores.push_back(game.getPlayer2Strategy()->getScore());
    }

    return { p1Scores, p2Scores };
}

template <typename T>
void TournamentManager<T>::runTournament() {
    std::vector<std::string> stratList = options.strategies;
    std::map<std::pair<std::string, std::string>, MatchStatistics> allResults;

    for (size_t i = 0; i < stratList.size(); ++i) {
        for (size_t j = i + 1; j < stratList.size(); ++j) {
            auto [p1Scores, p2Scores] = runIPD(stratList[i], stratList[j]);
            
            MatchStatistics stats = calculateStatistics(p1Scores, p2Scores);
            
            outputPairwisePayoffsStats(stratList[i], stratList[j], stats);
            
            allResults[{stratList[i], stratList[j]}] = stats;

            // Populate reverse entries
            MatchStatistics statsReverse = stats;
            std::swap(statsReverse.p1Mean, statsReverse.p2Mean);
            std::swap(statsReverse.p1Stdev, statsReverse.p2Stdev);
            std::swap(statsReverse.p1CILower, statsReverse.p2CILower);
            std::swap(statsReverse.p1CIUpper, statsReverse.p2CIUpper);

            allResults[{stratList[j], stratList[i]}] = statsReverse;
        }
    }

    writePairwisePayoffsFile(allResults);
    writePayoffMatrixFile(stratList, allResults);
    writeLeaderboardFile(stratList, allResults);

    std::cout << "\n- Files located at: x64 -> Debug folder\n";
    std::cout << "\n=========TOURNAMENT CONCLUDED=============================================================\n";
}

//template <typename T>
// ------------- !!! NEEDS FIXING !!! -------------
//void TournamentManager<T>::runEvolutionaryTournament() {
//    std::cout << "\n===== EVOLUTIONARY TOURNAMENT START =====\n";
//
//    int numStrats = static_cast<int>(options.strategies.size());
//
//    // Initialize equal population shares as doubles
//    std::map<std::string, double> populationShares;
//    double initialShare = static_cast<double>(options.population) / numStrats;
//    for (auto& strat : options.strategies)
//        populationShares[strat] = initialShare;
//
//    for (int gen = 1; gen <= options.generations; ++gen) {
//        std::cout << "\n-- Generation " << gen << " --\n";
//
//        // 1. Calculate average payoffs for each strategy
//        std::map<std::string, double> avgPayoffs;
//        for (auto& s1 : options.strategies) {
//            double totalScore = 0.0;
//            double totalWeight = 0.0;
//
//            for (auto& s2 : options.strategies) {
//                if (s1 == s2) continue;
//
//                auto [p1Scores, p2Scores] = runIPD(s1, s2);
//                double meanScore = std::accumulate(p1Scores.begin(), p1Scores.end(), 0.0) / p1Scores.size();
//
//                totalScore += meanScore * populationShares[s2];
//                totalWeight += populationShares[s2];
//
//                // DEBUG: Print match result
//                std::cout << "Match " << s1 << " vs " << s2
//                    << " -> meanScore: " << meanScore
//                    << " weighted by opponent share: " << meanScore * populationShares[s2] << "\n";
//            }
//
//            avgPayoffs[s1] = (totalWeight > 0.0) ? (totalScore / totalWeight) : 0.0;
//        }
//
//        // DEBUG: Print average payoffs per strategy
//        std::cout << "Average payoffs:\n";
//        for (auto& [s, f] : avgPayoffs)
//            std::cout << "  " << s << ": " << f << "\n";
//
//        // 2. Compute new shares proportionally
//        double totalFitness = 0.0;
//        for (auto& [s, fitness] : avgPayoffs)
//            totalFitness += fitness;
//
//        std::map<std::string, double> newPopDouble;
//        if (totalFitness > 0.0) {
//            for (auto& [s, fitness] : avgPayoffs) {
//                double proportion = fitness / totalFitness;
//                newPopDouble[s] = proportion * options.population;
//            }
//        }
//        else {
//            // all fitness zero -> equal shares
//            for (auto& s : options.strategies)
//                newPopDouble[s] = populationShares[s];
//        }
//
//        // 3. Convert to int safely
//        std::map<std::string, int> newPopulation;
//        int sumShares = 0;
//        std::vector<std::pair<std::string, double>> fractions;
//
//        for (auto& [s, share] : newPopDouble) {
//            int intShare = static_cast<int>(std::floor(share));
//            newPopulation[s] = intShare;
//            sumShares += intShare;
//            fractions.emplace_back(s, share - intShare); // store fractional part
//        }
//
//        // 4. Distribute remaining population
//        int remaining = options.population - sumShares;
//        std::sort(fractions.begin(), fractions.end(),
//            [](auto& a, auto& b) { return a.second > b.second; });
//        for (int i = 0; i < remaining; ++i)
//            newPopulation[fractions[i % fractions.size()].first]++;
//
//        // 5. Update populationShares for next generation
//        for (auto& [s, count] : newPopulation)
//            populationShares[s] = static_cast<double>(count);
//
//        // Print population summary
//        std::cout << "Population shares:\n";
//        for (auto& [s, share] : newPopulation)
//            std::cout << "  " << s << ": " << share << "\n";
//    }
//
//    std::cout << "\n===== EVOLUTIONARY TOURNAMENT END =====\n";
//}