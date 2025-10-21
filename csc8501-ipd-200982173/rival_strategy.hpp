#pragma once
#include <string>
#include "strategy.hpp"
#include "game_state.hpp"

class RIVAL : public Strategy {
public:
    RIVAL();

    Action decideAction(const GameState& state) override;
    std::string name() const override;
private:
    double scoreDiffThreshold;
    bool catchupActive;
    bool recoveryCoop;
    Action lastOpponentMove;
};
