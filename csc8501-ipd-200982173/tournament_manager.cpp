#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>
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

void TournamentManager::runTournament() {
    std::vector<std::string> stratList = options.strategies;
    std::string csvFileName = createFilename();

    std::ofstream csv(csvFileName);
    if (!csv.is_open()) {
        throw std::runtime_error("Results file " + csvFileName + " was not created");
    }

    csv << "Rounds: " << options.rounds << "\n";
    csv << "Repetitions: " << options.repeats << "\n";
    csv << "Payoff: " << payoff.getT() << "," << payoff.getR() << "," << payoff.getP() << "," << payoff.getS() << "\n";
    csv << "\n\n";
    csv << "Strategy[1],Strategy[2]," << "Mean[1],Mean[2]," << "Stdev[1],Stdev[2]," 
        << "CI_Low[1],CI_Up[1]," << "CI_Low[2],CI_Up[2]\n";

    for (size_t i = 0; i < stratList.size(); ++i) {
        for (size_t j = i + 1; j < stratList.size(); ++j) {
            std::vector<double> p1Scores;
            std::vector<double> p2Scores;

            for (int r = 0; r < options.repeats; ++r) {
                auto p1Strategy = StrategyCreator::createStrategy(stratList[i]);
                auto p2Strategy = StrategyCreator::createStrategy(stratList[j]);
                Player p1(1, p1Strategy->name());
                Player p2(2, p2Strategy->name());

                GameManager game(std::move(p1Strategy), std::move(p2Strategy), p1, p2, payoff);
                game.runGame(options.rounds, r + 1, options.repeats);

                p1Scores.push_back(p1.getScore());
                p2Scores.push_back(p2.getScore());
            }

			// Calculate mean - lambda function
            auto mean = [](const std::vector<double>& scores) {
                if (scores.empty()) {
                    return 0.0;
                }
                double sum = 0;
                for (double score : scores) {
                    sum += score;
                }
                return sum / scores.size();
            };

            double p1Mean = mean(p1Scores);
            double p2Mean = mean(p2Scores);

            // Calculate standard deviation - lambda function
            auto stdev = [&](const std::vector<double>& scores, double mean) {
                if (scores.size() <= 1) {
                    return 0.0;
                }
                double sumSquared = 0;
                for (double score : scores) {
                    sumSquared += (score - mean) * (score - mean);
                }
                return std::sqrt(sumSquared / (scores.size() - 1));
            };

            double p1Stdev = stdev(p1Scores, p1Mean);
            double p2Stdev = stdev(p2Scores, p2Mean);

            std::string p1CiLowerStr, p1CiUpperStr, p2CiLowerStr, p2CiUpperStr;

            if (options.repeats > 1) {
                double p1StandardError = p1Stdev / std::sqrt(options.repeats);
                double p2StandardError = p2Stdev / std::sqrt(options.repeats);

                double p1CiRange = 1.96 * p1StandardError;
                double p2CiRange = 1.96 * p2StandardError;

                double p1CiLower = p1Mean - p1CiRange;
                double p1CiUpper = p1Mean + p1CiRange;
                double p2CiLower = p2Mean - p2CiRange;
                double p2CiUpper = p2Mean + p2CiRange;

                p1CiLowerStr = std::to_string(p1CiLower);
                p1CiUpperStr = std::to_string(p1CiUpper);
                p2CiLowerStr = std::to_string(p2CiLower);
                p2CiUpperStr = std::to_string(p2CiUpper);
            }
            else {
                // When --repeats is 1
                p1CiLowerStr = "N/A";
                p1CiUpperStr = "N/A";
                p2CiLowerStr = "N/A";
                p2CiUpperStr = "N/A";
            }

            // Console output
            std::cout << "\n" << stratList[i] << " vs " << stratList[j] << ":\n";
            std::cout << "  Player 1 (" << stratList[i] << ") Mean = " << p1Mean << ", 95% CI [" << p1CiLowerStr << ", " << p1CiUpperStr << "]\n";
            std::cout << "  Player 2 (" << stratList[j] << ") Mean = " << p2Mean << ", 95% CI [" << p2CiLowerStr << ", " << p2CiUpperStr << "]\n";
            std::cout << "-------------------------------------\n";

            // Write to CSV
            csv << stratList[i] << "," << stratList[j] << "," << p1Mean << "," << p2Mean << "," << p1Stdev << "," << p2Stdev << "," 
                << p1CiLowerStr << "," << p1CiUpperStr << "," << p2CiLowerStr << "," << p2CiUpperStr << "\n";
        }
    }

    csv.close();
    std::cout << "\nTournament finished. Results available in " << csvFileName << "\n";
    std::cout << "Please find this in: x64 -> Debug folder\n";
} 