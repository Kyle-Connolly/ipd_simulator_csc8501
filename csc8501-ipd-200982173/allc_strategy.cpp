#include "allc_strategy.hpp"

Action ALLC::decideAction() {
    return Action::Cooperate;
}

std::string ALLC::name() const {
    return "ALLC";
}