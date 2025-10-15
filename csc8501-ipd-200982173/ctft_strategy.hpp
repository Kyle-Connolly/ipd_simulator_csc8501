#pragma once
#include "strategy.hpp"

class CTFT : public Strategy {
public:
    
    Action decideAction(const GameState& state) override;
    std::string name() const override;
    bool isContrite() const { return contrite; }
    void setLastMoves(Action intended, Action actual);
    
private:
    bool contrite = false;
    Action lastIntendedAction = Action::Cooperate;
    Action lastActualAction = Action::Cooperate;
};