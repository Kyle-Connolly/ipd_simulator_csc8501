#include "rnd_strategy.hpp"

RND::RND(double probability)
    : p(probability), randNum(std::random_device{}()), rangeLimit(0.0, 1.0) {
}

// Randomly decide each round based on the value of p
Action RND::decideAction(const GameState&) {
    double randomValue = rangeLimit(randNum);
    if (randomValue < p) {
        return Action::Cooperate;
    }
    else {
        return Action::Defect;
    }
}

std::string RND::name() const {
    return "RND";
}