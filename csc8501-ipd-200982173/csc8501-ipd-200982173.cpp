#include <iostream>
#include <memory>
#include "allc_strategy.hpp"
#include "alld_strategy.hpp"
#include "tft_strategy.hpp"
#include "grim_strategy.hpp"
#include "pavlov_strategy.hpp"
#include "rnd_strategy.hpp"
#include "game_manager.hpp"

int main(int argc, char* argv[])
{
    auto strategy1 = std::make_unique<RND>(0.5);
    auto strategy2 = std::make_unique<ALLC>();

    Player p1(1, strategy1->name());
    Player p2(2, strategy2->name());

    Payoff payoff(5, 3, 1, 0); // T, R, P, S

    GameManager ipdGame(std::move(strategy1), std::move(strategy2), p1, p2, payoff);
    ipdGame.runGame(5);
}