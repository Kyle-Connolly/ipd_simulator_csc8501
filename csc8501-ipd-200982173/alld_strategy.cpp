#include "alld_strategy.hpp"

Action ALLD::decideAction() {
    return Action::Defect;
}

std::string ALLD::name() const {
    return "ALLD";
}