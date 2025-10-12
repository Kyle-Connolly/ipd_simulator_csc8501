#include "alld_strategy.hpp"

Action ALLD::decideAction(const GameState&) {
    return Action::Defect;
}

std::string ALLD::name() const {
    return "ALLD";
}