#include "allc_strategy.hpp"

Action ALLC::decideAction(const GameState&) {
    return Action::Cooperate;
}

std::string ALLC::name() const {
    return "ALLC";
}