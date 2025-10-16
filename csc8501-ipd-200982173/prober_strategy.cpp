#include "prober_strategy.hpp"
#include "game_state.hpp"

Action PROBER::decideAction(const GameState& state) {
    // Reset at start of new match
    if (state.firstRound) {
        opponentHistory.clear();
        exploitable = -1;
    }

    if (state.roundNum > 1 && state.roundNum < 5) {
        opponentHistory.push_back(state.lastOpponentMove);
    }

    if (state.roundNum >= 1 && state.roundNum <= 4) {
        opponentHistory.push_back(state.lastOpponentMove);

        // Round 4 - determine exploitability based on opponent’s behavior after defection
        if (state.roundNum == 4 && exploitable == -1) {
            // If opponent cooperated after defection - exploitable
            // If opponent defected - unexploitable
            exploitable = (state.lastOpponentMove == Action::Cooperate) ? 1 : 0;
        }

        return probeSequence[state.roundNum - 1];
    }

	// If not decided, then 0 = not exploitable and perform TFT functionality
    if (exploitable == -1) {
        exploitable = 0;
    }

    // If exploitable defect forever
    if (exploitable == 1) {
        return Action::Defect;
    }

    // Unexploitable but Cooperate first after probe, then TFT
    if (state.roundNum == 4) {
        // Check if opponent defected in every probe round
        bool allDefected = true;
        for (const auto& action : opponentHistory) {
            if (action != Action::Defect) {
                allDefected = false; // Find any action that isn't a defect
                break;               
            }
        }

        if (allDefected) {
            exploitable = 0;
            return Action::Defect;
        }

        return Action::Cooperate;
    }

    // TFT functionality
    return state.lastOpponentMove;
}

std::string PROBER::name() const {
    return "PROBER";
}
