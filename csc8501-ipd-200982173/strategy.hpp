#pragma once
#include <string>
#include "action.hpp"

struct GameState;

class Strategy {
public:
    virtual Action decideAction(const GameState& state) = 0;
    virtual std::string name() const = 0;
    virtual ~Strategy() = default; //destructor
    double getScore() const { return score; }
    void addScore(double s) { score += s; }
    void resetScore() { score = 0; }

protected:
    double score = 0;
};