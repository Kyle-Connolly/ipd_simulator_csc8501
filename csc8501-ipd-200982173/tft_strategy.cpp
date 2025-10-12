#include "tft_strategy.hpp"
#include "game_state.hpp" 

Action TFT::decideAction(const GameState& state) {
    // Round 1 = always cooperate
    if (state.firstRound) {
        return Action::Cooperate;
    }
    return state.lastOpponentMove;
}

std::string TFT::name() const {
    return "TFT";
}