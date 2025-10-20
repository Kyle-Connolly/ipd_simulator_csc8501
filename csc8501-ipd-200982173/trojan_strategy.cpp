#include "trojan_strategy.hpp"

TROJAN::TROJAN()
    : opponentHistory(), exploitable(-1), opponentDefects(0), coopRounds(5), probeRound(6), exploitFailThreshold(3), coopModeRound(0), coopMode(false), randNumGen(std::random_device{}()), 
    rangeLimit(5, 10)                    
{
    coopRounds = rangeLimit(randNumGen);
    probeRound = coopRounds + 1;
}

Action TROJAN::decideAction(const GameState& state) {
    // Reset
    if (state.firstRound) {
        opponentHistory.clear();
        exploitable = -1;
        opponentDefects = 0;
        coopModeRound = 0;
        coopMode = false;
        std::uniform_int_distribution<int> dist(5, 10);
        coopRounds = dist(randNumGen);
        probeRound = coopRounds + 1;
    }
	// Start logging opponent history
    if (state.roundNum > 1) {
        opponentHistory.push_back(state.lastOpponentMove);
    }

	// Stage 1. Cooperate for coopRounds rounds
    if (state.roundNum <= coopRounds) {
        return Action::Cooperate;
    }

    // Stage 2. Probe round (defect)
    if (state.roundNum == probeRound) {
        return Action::Defect;
    }

    // Stage 3: Determine if exploitable
    if (state.roundNum == probeRound + 1 && exploitable == -1) {
        exploitable = (state.lastOpponentMove == Action::Cooperate) ? 1 : 0;
    }

	// Failed exploitation so need to attempt cooperation for 3 rounds
	// If not opponent does not forgive, switch to TFT
    if (coopMode) {
        coopModeRound++;
        if (coopModeRound >= 3 && state.lastOpponentMove != Action::Cooperate) {
            coopMode = false;
            exploitable = 0;
            return state.lastOpponentMove; // switch to TFT behaviour
        }
        if (coopModeRound >= 3 && state.lastOpponentMove == Action::Cooperate) {
            coopMode = false;
            exploitable = 1;
            return Action::Defect;
        }
        return Action::Cooperate;
    }

    // Exploit opponent
    if (exploitable == 1) {
        if (state.lastOpponentMove == Action::Defect) {
            opponentDefects++;
        }
        else {
            opponentDefects = 0;
        }
		// Opponent is retaliating, stop exploiting and cooperate
        if (opponentDefects >= exploitFailThreshold) {
            coopMode = true;
            coopModeRound = 0;
            return Action::Cooperate;
        }

        return Action::Defect; // continue exploiting
    }

    return state.lastOpponentMove;
}

std::string TROJAN::name() const {
    return "TROJAN";
}
