#include "ctft_strategy.hpp"
#include "game_state.hpp"

Action CTFT::decideAction(const GameState& state) {
    // Round 1 = always cooperate
    if (state.firstRound) {
        contrite = false;
        lastIntendedAction = Action::Cooperate;
        lastActualAction = Action::Cooperate;

        return Action::Cooperate;
    }

    // If contrite keep cooperating until opponent cooperates again
    if (contrite) {
        // If opponent cooperated last round, contrition ends, return to TFT behaviour
        if (state.lastOpponentMove == Action::Cooperate) {
            contrite = false;
        }
        else {
            return Action::Cooperate;
        }
    }

    // TFT behaviour
    Action nextAction = state.lastOpponentMove;

	// Accidental defection in previous round due to noise. Intended to cooperate, but noise flipped to Defect
    // Opponent retaliated with Defect so need to apologise (cooperate).
    if (lastIntendedAction == Action::Cooperate && lastActualAction == Action::Defect && state.lastOpponentMove == Action::Defect) {
        contrite = true;
        return Action::Cooperate;
    }

    return nextAction;
}

std::string CTFT::name() const {
    return "CTFT";
}

void CTFT::setLastMoves(Action intended, Action actual) {
    lastIntendedAction = intended;
    lastActualAction = actual;
}

