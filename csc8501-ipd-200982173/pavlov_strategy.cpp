#include "pavlov_strategy.hpp"
#include "game_state.hpp" 

Action PAVLOV::decideAction(const GameState& state) {
    if (state.firstRound) {
        return Action::Cooperate;
    }
    // If previous moves matched (CC or DD) then stay, else switch if different (CD or DC)
    if (state.lastMove == state.lastOpponentMove) {
        return state.lastMove;
    }
    else {
        return (state.lastMove == Action::Cooperate) ? Action::Defect : Action::Cooperate;
    }
}

std::string PAVLOV::name() const {
    return "PAVLOV";
}