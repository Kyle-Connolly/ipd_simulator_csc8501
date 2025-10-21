#include "rival_strategy.hpp"

RIVAL::RIVAL() : scoreDiffThreshold(5.0), catchupActive(false), recoveryCoop(false), lastOpponentMove(Action::Cooperate)
{}

Action RIVAL::decideAction(const GameState& state) {
    if (state.firstRound) {
        catchupActive = false;
        recoveryCoop = false;
        lastOpponentMove = Action::Cooperate;
        return Action::Cooperate;
    }

    double scoreDiff = state.playerScore - state.opponentScore;

    if (scoreDiff <= -scoreDiffThreshold || (scoreDiff == 0 && catchupActive)) {
		catchupActive = true; // Falling behind so start catchup behaviour
        recoveryCoop = false;
    }
    else if (scoreDiff >= scoreDiffThreshold && catchupActive) {
        catchupActive = false;
        recoveryCoop = true;  // Cooperate before returning to TFT behaviour
    }

    lastOpponentMove = state.lastOpponentMove;

    if (catchupActive) {
		return Action::Defect; // Need to catch up to opponent so always defect
    }
    else {
        // TFT mode
        if (recoveryCoop) {
            recoveryCoop = false;
            return Action::Cooperate;
        }
        return state.lastOpponentMove;
    }
}

std::string RIVAL::name() const {
    return "RIVAL";
}

