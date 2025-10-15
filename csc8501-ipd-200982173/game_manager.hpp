#pragma once
#include <memory>
#include <random>
#include "payoff.hpp"
#include "strategy.hpp"

class GameManager {
public:
    GameManager(std::unique_ptr<Strategy> s1, std::unique_ptr<Strategy> s2, const Payoff& payoff, double epsilon = 0.0, int seed = 0, bool noiseOn = false);
    void runGame(int rounds, int repetition, int totalRepeats);
    void printResults() const;
	std::unique_ptr<Strategy> getPlayer1Strategy() { return std::move(player1Strategy); }
	std::unique_ptr<Strategy> getPlayer2Strategy() { return std::move(player2Strategy); }

private:
    std::unique_ptr<Strategy> player1Strategy;
    std::unique_ptr<Strategy> player2Strategy;
    const Payoff& payoffSystem;
    double epsilon;
    bool noiseOn;
    std::mt19937 randomNumGenerator;
    std::uniform_real_distribution<double> distribution{ 0.0, 1.0 };
};