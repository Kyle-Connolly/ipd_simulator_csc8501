#pragma once
#include <memory>
#include <random>
#include "payoff.hpp"
#include "strategy.hpp"

template <typename T>
class GameManager {
public:
    GameManager(std::unique_ptr<Strategy> s1, std::unique_ptr<Strategy> s2, const Payoff<T>& payoff, double epsilon, std::mt19937& randNumGen, bool noiseOn, const std::string& outputFormat);
    void runGame(int rounds, int repetition, int totalRepeats);
    void printResults() const;
    const Strategy* getPlayer1Strategy() { return player1Strategy.get(); }
    const Strategy* getPlayer2Strategy() { return player2Strategy.get(); }

private:
    std::unique_ptr<Strategy> player1Strategy;
    std::unique_ptr<Strategy> player2Strategy;
    const Payoff<T>& payoffSystem;
    double epsilon;
    bool noiseOn;  
    std::mt19937& randNumGen;
    std::uniform_real_distribution<double> distribution{ 0.0, 1.0 };
    std::string outputFormat;
};

#include "game_manager.tpp"