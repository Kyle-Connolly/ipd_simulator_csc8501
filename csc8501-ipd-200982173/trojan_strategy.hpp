#pragma once
#include <vector>
#include <random>
#include "strategy.hpp"
#include "game_state.hpp"

class TROJAN : public Strategy {
public:
    TROJAN();

    Action decideAction(const GameState& state) override;
    std::string name() const override;

private:
    std::mt19937 randNumGen;
    std::uniform_int_distribution<int> rangeLimit;
    std::vector<Action> opponentHistory;
    int exploitable; // -1 = undecided, 0 = not exploitable, 1 = exploitable
    int opponentDefects;
    int coopRounds; // // Random cooperative phase between rounds 5 and 10
    int probeRound;    // After coopRounds, one probe round (defect)
    const int exploitFailThreshold; // If opponent defects 3 consecutive rounds then stop exploiting
    int coopModeRound; // Track cooperative phase rounds 
    bool coopMode;
};
