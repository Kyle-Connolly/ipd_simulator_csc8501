#include "grim_strategy.hpp"

Action GRIM::decideAction() {
    // Round 1 = Cooperate
    if (opponentHistory.empty()) {
        return Action::Cooperate;
    }

    // Check if opponent defected
    for (const auto& action : opponentHistory) {
        if (action == Action::Defect) {
            opponentDefected = true;
            break;
        }
    }

    // If opponent defects, always defect
    if (opponentDefected) {
        return Action::Defect;
    }

    // Keep cooperating otherwise
    return Action::Cooperate;
}

std::string GRIM::name() const {
    return "GRIM";
}