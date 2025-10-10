#include "tft_strategy.hpp"

Action TFT::decideAction() {
    // Round 1 = always cooperate
    if (opponentHistory.empty()) {
        return Action::Cooperate;
    }

    // Later rounds: copy opponent’s previous move
    return opponentHistory.back();
}

std::string TFT::name() const {
    return "TFT";
}