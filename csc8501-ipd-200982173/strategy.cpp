#include "strategy.hpp"

void Strategy::recordOutcome(Action ownAction, Action opponentAction) {
    playerHistory.push_back(ownAction);
    opponentHistory.push_back(opponentAction);
}

void Strategy::reset() {
    playerHistory.clear();
    opponentHistory.clear();
}