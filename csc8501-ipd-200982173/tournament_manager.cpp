#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <cmath>
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

std::pair<std::vector<double>, std::vector<double>> TournamentManager::runMatchup(
    const std::string& strat1, const std::string& strat2) {

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

MatchStatistics TournamentManager::calculateStatistics(
    const std::vector<double>& p1Scores, const std::vector<double>& p2Scores) {

    MatchStatistics stats;

    // Define lambdas for mean and stdev
    auto calculateMean = [](const std::vector<double>& scores) -> double {
        if (scores.empty()) return 0.0;
        double sum = 0.0;
        for (double s : scores) sum += s;
        return sum / scores.size();
        };

    auto calculateStdev = [](const std::vector<double>& scores, double mean) -> double {
        if (scores.size() <= 1) return 0.0;
        double sumSquared = 0.0;
        for (double s : scores) sumSquared += (s - mean) * (s - mean);
        return std::sqrt(sumSquared / (scores.size() - 1));
        };

    // Use lambdas to compute stats
    stats.p1Mean = calculateMean(p1Scores);
    stats.p2Mean = calculateMean(p2Scores);

    stats.p1Stdev = calculateStdev(p1Scores, stats.p1Mean);
    stats.p2Stdev = calculateStdev(p2Scores, stats.p2Mean);

    // Confidence interval calculations (same as before)
    if (options.repeats > 1) {
        double p1StandardError = stats.p1Stdev / std::sqrt(options.repeats);
        double p2StandardError = stats.p2Stdev / std::sqrt(options.repeats);

        double p1CiRange = 1.96 * p1StandardError;
        double p2CiRange = 1.96 * p2StandardError;

        stats.p1CiLowerStr = std::to_string(stats.p1Mean - p1CiRange);
        stats.p1CiUpperStr = std::to_string(stats.p1Mean + p1CiRange);
        stats.p2CiLowerStr = std::to_string(stats.p2Mean - p2CiRange);
        stats.p2CiUpperStr = std::to_string(stats.p2Mean + p2CiRange);
    }
    else {
        stats.p1CiLowerStr = "N/A";
        stats.p1CiUpperStr = "N/A";
        stats.p2CiLowerStr = "N/A";
        stats.p2CiUpperStr = "N/A";
    }

    return stats;
}

void TournamentManager::writeResults(std::ofstream& csv,
    const std::string& strat1,
    const std::string& strat2,
    const MatchStatistics& stats) {
    // Console output
    std::cout << "\n" << strat1 << " vs " << strat2 << ":\n";
    std::cout << " Player 1 (" << strat1 << ") Mean = " << stats.p1Mean
        << ", 95% CI [" << stats.p1CiLowerStr << ", " << stats.p1CiUpperStr << "]\n";
    std::cout << " Player 2 (" << strat2 << ") Mean = " << stats.p2Mean
        << ", 95% CI [" << stats.p2CiLowerStr << ", " << stats.p2CiUpperStr << "]\n";
    std::cout << "-------------------------------------\n";

    // CSV output
    csv << strat1 << "," << strat2 << ","
        << stats.p1Mean << "," << stats.p2Mean << ","
        << stats.p1Stdev << "," << stats.p2Stdev << ","
        << stats.p1CiLowerStr << "," << stats.p1CiUpperStr << ","
        << stats.p2CiLowerStr << "," << stats.p2CiUpperStr << "\n";
}

void TournamentManager::runTournament() {
    std::vector<std::string> stratList = options.strategies;
    std::string csvFileName = createFilename();

    std::ofstream csv(csvFileName);
    if (!csv.is_open()) {
        throw std::runtime_error("Results file " + csvFileName + " was not created");
    }

    // Tournament header
    csv << "Rounds: " << options.rounds << "\n";
    csv << "Repetitions: " << options.repeats << "\n";
    csv << "Payoff: " << payoff.getT() << "," << payoff.getR() << ","
        << payoff.getP() << "," << payoff.getS() << "\n\n\n";

    csv << "Strategy[1],Strategy[2],Mean[1],Mean[2],Stdev[1],Stdev[2],"
        "CI_Low[1],CI_Up[1],CI_Low[2],CI_Up[2]\n";

    // Round-robin tournament loop
    for (size_t i = 0; i < stratList.size(); ++i) {
        for (size_t j = i + 1; j < stratList.size(); ++j) {
            auto [p1Scores, p2Scores] = runMatchup(stratList[i], stratList[j]);
            MatchStatistics stats = calculateStatistics(p1Scores, p2Scores);
            writeResults(csv, stratList[i], stratList[j], stats);
        }
    }

    csv.close();

    std::cout << "\nTournament finished. Results available in " << csvFileName << "\n";
    std::cout << "Please find this in: x64 -> Debug folder\n";
}
