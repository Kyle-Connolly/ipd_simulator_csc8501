#pragma once

#include "strategy.hpp"

class ALLD : public Strategy {
public:
    Action decideAction(const GameState& state) override;
    std::string name() const override;
};