#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <map>
#include "tournament_manager.hpp"
#include "game_manager.hpp"
#include "player.hpp"
#include "strategy_creator.hpp"

TournamentManager::TournamentManager(const CommandOptions& options, const Payoff& payoff)
    : options(options), payoff(payoff) {
}

std::string TournamentManager::createFilename() const {
    auto systemTime = std::chrono::system_clock::now();
    std::time_t systemTimeConversion = std::chrono::system_clock::to_time_t(systemTime);
    std::tm dateTime{};

#ifdef _WIN32
    localtime_s(&dateTime, &systemTimeConversion);
#else
    localtime_r(&systemTimeConversion, &dateTime);
#endif

    std::ostringstream filename;
    filename << "results_" << std::put_time(&dateTime, "%Y%m%d_%H%M%S") << ".csv";
    return filename.str();
}

std::pair<std::vector<double>, std::vector<double>> TournamentManager::runIPD(const std::string& strat1, const std::string& strat2) {
    std::vector<double> p1Scores;
    std::vector<double> p2Scores;

    for (int r = 0; r < options.repeats; ++r) {
        auto p1Strategy = StrategyCreator::createStrategy(strat1);
        auto p2Strategy = StrategyCreator::createStrategy(strat2);

        Player p1(1, p1Strategy->name());
        Player p2(2, p2Strategy->name());

        GameManager game(std::move(p1Strategy), std::move(p2Strategy), p1, p2, payoff);
        game.runGame(options.rounds, r + 1, options.repeats);

        p1Scores.push_back(p1.getScore());
        p2Scores.push_back(p2.getScore());
    }

    return { p1Scores, p2Scores };
}

MatchStatistics TournamentManager::calculateStatistics(const std::vector<double>& p1Scores, const std::vector<double>& p2Scores) {
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

void TournamentManager::writeResults(std::ofstream& csv, const std::string& strat1, const std::string& strat2, const MatchStatistics& stats) {
	// Console output
	std::cout << "\n---------CONFIDENCE INTERVAL---------";
    std::cout << "\n" << strat1 << " vs " << strat2 << ":\n";
    std::cout << " Player 1 (" << strat1 << ") Mean = " << stats.p1Mean << ", 95% CI [" << stats.p1CILower << ", " << stats.p1CIUpper << "]\n";
    std::cout << " Player 2 (" << strat2 << ") Mean = " << stats.p2Mean << ", 95% CI [" << stats.p2CILower << ", " << stats.p2CIUpper << "]\n";
    std::cout << "==========================================================================================\n";

    // CSV output
    csv << strat1 << "," << strat2 << ","
        << stats.p1Mean << "," << stats.p2Mean << ","
        << stats.p1Stdev << "," << stats.p2Stdev << ","
        << stats.p1CILower << "," << stats.p1CIUpper << ","
        << stats.p2CILower << "," << stats.p2CIUpper << "\n";
}

std::ofstream TournamentManager::openResultsFile(std::string& outFilename) const {
    outFilename = createFilename();
    std::ofstream csv(outFilename);

    if (!csv.is_open()) {
        throw std::runtime_error("Results file " + outFilename + " was not created");
    }

    return csv;
}

void TournamentManager::writeResultsFileHeader(std::ofstream& csv) const {
    csv << "Rounds: " << options.rounds << "\n";
    csv << "Repetitions: " << options.repeats << "\n";
    csv << "Payoff: " << payoff.getT() << "," << payoff.getR() << "," << payoff.getP() << "," << payoff.getS() << "\n\n\n";
    csv << "Strategy[1],Strategy[2],Mean[1],Mean[2],Stdev[1],Stdev[2],CI_Low[1],CI_Up[1],CI_Low[2],CI_Up[2]\n";
}

void TournamentManager::writePayoffMatrixFile(const std::vector<std::string>& strategies, const std::map<std::pair<std::string, std::string>, MatchStatistics>& results) const {
    std::string filename = "payoff_matrix_" + createFilename();
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

	std::cout << "IPD Tournament Complete.";
    std::cout << "\n- Payoff matrix saved in: " << filename;
}

void TournamentManager::runTournament() {
    std::vector<std::string> stratList = options.strategies;
    std::string csvFileName;

    std::ofstream csv = openResultsFile(csvFileName);
    writeResultsFileHeader(csv);

    // For payoff matrix 
    std::map<std::pair<std::string, std::string>, MatchStatistics> allResults;

    for (size_t i = 0; i < stratList.size(); ++i) {
        for (size_t j = i + 1; j < stratList.size(); ++j) {
            auto [p1Scores, p2Scores] = runIPD(stratList[i], stratList[j]);
            MatchStatistics stats = calculateStatistics(p1Scores, p2Scores);
            writeResults(csv, stratList[i], stratList[j], stats);

            allResults[{stratList[i], stratList[j]}] = stats;

			// Also store the reverse match for the payoff matrix
            MatchStatistics statsReverse{};
            statsReverse.p1Mean = stats.p2Mean;
            statsReverse.p2Mean = stats.p1Mean;
            allResults[{stratList[j], stratList[i]}] = statsReverse;
        }
    }

    csv.close();
    writePayoffMatrixFile(stratList, allResults);

    std::cout << "\n- Pairwise payoffs results saved in: " << csvFileName;
    std::cout << "\n- Results + Payoff Matrix files location: x64 -> Debug folder\n";
}
