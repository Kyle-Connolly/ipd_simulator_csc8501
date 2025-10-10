#pragma once

#include <vector>
#include <string>

enum class Action { Cooperate, Defect };

class Strategy {
protected:
    std::vector<Action> playerHistory;
    std::vector<Action> opponentHistory;

public:
    virtual Action decideAction() = 0;
    virtual void recordOutcome(Action ownAction, Action opponentAction);
    virtual void reset();
    virtual std::string name() const = 0;
    virtual ~Strategy() = default; //destructor
};