#pragma once

#include "strategy.hpp"

class GRIM : public Strategy {
public:
    Action decideAction() override;
    std::string name() const override;
private:
    bool opponentDefected = false; // Track if opponent has ever defected
};