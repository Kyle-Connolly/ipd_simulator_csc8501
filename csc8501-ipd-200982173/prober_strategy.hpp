#pragma once
#include <vector>
#include "strategy.hpp"

class PROBER : public Strategy {
public:
    Action decideAction(const GameState& state) override;
    std::string name() const override;
    std::vector<Action> opponentHistory;

private:
    // Fixed probe sequence for rounds 1 to 4 (C, D, C, C).
    static constexpr Action probeSequence[4] = {
        Action::Cooperate,
        Action::Defect,
        Action::Cooperate,
        Action::Cooperate
    };

    // To classify opponent as exploitable: -1 = undecided, 0 = not exploitable, 1 = exploitable
    int exploitable = -1;
};
