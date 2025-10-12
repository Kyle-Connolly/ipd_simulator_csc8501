#pragma once
#include "action.hpp"

// State of the current game round
struct GameState {
    int roundNum;
    bool firstRound;
    bool opponentDefected;
    Action lastMove;
    Action lastOpponentMove;
};