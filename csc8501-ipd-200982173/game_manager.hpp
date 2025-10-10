#pragma once

#include <memory>
#include "player.hpp"
#include "payoff.hpp"
#include "strategy.hpp"

class GameManager {
public:
    GameManager(std::unique_ptr<Strategy> s1, std::unique_ptr<Strategy> s2, Player p1, Player p2, Payoff payoff);

    void runGame(int rounds);

private:
    std::unique_ptr<Strategy> player1Strategy;
    std::unique_ptr<Strategy> player2Strategy;
    Player player1;
    Player player2;
    Payoff payoffSystem;

    void printResults();
};