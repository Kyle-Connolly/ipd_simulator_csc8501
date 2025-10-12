#include "pavlov_strategy.hpp"
#include "game_state.hpp" 

Action PAVLOV::decideAction(const GameState& state) {
    if (state.firstRound) {
        return Action::Cooperate;
    }
    bool movesMatch = (state.lastMove == state.lastOpponentMove);
    return movesMatch ? Action::Cooperate : Action::Defect;
}

std::string PAVLOV::name() const {
    return "PAVLOV";
}