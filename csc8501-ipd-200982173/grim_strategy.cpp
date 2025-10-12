#include "grim_strategy.hpp"
#include "game_state.hpp" 

Action GRIM::decideAction(const GameState& state) {
    if (state.firstRound) {
        return Action::Cooperate;
    }
    if (state.opponentDefected) {
        return Action::Defect;
    }
    return Action::Cooperate;
}

std::string GRIM::name() const {
    return "GRIM";
}