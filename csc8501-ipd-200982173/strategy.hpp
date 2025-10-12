#pragma once

#include <string>
#include "action.hpp"

struct GameState;

class Strategy {
public:
    virtual Action decideAction(const GameState& state) = 0;
    virtual std::string name() const = 0;
    virtual ~Strategy() = default; //destructor
};