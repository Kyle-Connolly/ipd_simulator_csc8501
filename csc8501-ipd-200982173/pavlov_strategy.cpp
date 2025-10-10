#include "pavlov_strategy.hpp"

Action PAVLOV::decideAction() {
    // First round = default to cooperate
    if (playerHistory.empty()) {
        return Action::Cooperate;
    }

    // Get last actions
    Action myLast = playerHistory.back();
    Action oppLast = opponentHistory.back();

    // Determine next action
    if ((myLast == Action::Cooperate && oppLast == Action::Cooperate) ||
        (myLast == Action::Defect && oppLast == Action::Defect)) {
        // Win: Stay
        return myLast;
    }
    else {
        // Lose = switch actions
        return (myLast == Action::Cooperate) ? Action::Defect : Action::Cooperate;
    }
}

std::string PAVLOV::name() const {
    return "PAVLOV";
}