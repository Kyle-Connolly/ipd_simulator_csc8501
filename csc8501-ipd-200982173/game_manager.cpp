#include <iostream>
#include "game_manager.hpp"
#include "game_state.hpp" 

GameManager::GameManager(std::unique_ptr<Strategy> s1,std::unique_ptr<Strategy> s2, Player& p1, Player& p2, const Payoff& payoff)
    : player1Strategy(std::move(s1)),
    player2Strategy(std::move(s2)),
    player1(p1),
    player2(p2),
    payoffSystem(payoff) {}

void GameManager::runGame(int rounds, int repetition, int totalRepeats) {
    Action p1LastAction = Action::Cooperate;
    Action p2LastAction = Action::Cooperate;
    bool p1OpponentDefected = false;
    bool p2OpponentDefected = false;
    
	std::cout << "----------------------------------";
    std::cout << "\nNext match: " << player1.getStrategy() << " vs " << player2.getStrategy() << "\n";
    std::cout << "Repetition " << repetition << " of " << totalRepeats << " \n\n";
    

    for (int round = 1; round <= rounds; ++round) {
        GameState state1{
            round,
            (round == 1),
            p1OpponentDefected,
            p1LastAction,
            p2LastAction,
        };

        GameState state2{
            round,
            (round == 1),
            p2OpponentDefected,
            p2LastAction,
            p1LastAction,
        };
        
        Action p1Action = player1Strategy->decideAction(state1);
        Action p2Action = player2Strategy->decideAction(state2);

        // Update defection flags - used for GRIM and similar
        if (p2Action == Action::Defect)
            p1OpponentDefected = true;
        if (p1Action == Action::Defect)
            p2OpponentDefected = true;

        // Convert Action to bool for payoff calculation
        bool p1Cooperated = (p1Action == Action::Cooperate);
        bool p2Cooperated = (p2Action == Action::Cooperate);

        double p1Score = payoffSystem.calculatePayoff(p1Cooperated, p2Cooperated);
        double p2Score = payoffSystem.calculatePayoff(p2Cooperated, p1Cooperated);

        // Update scores
        player1.addScore(static_cast<int>(p1Score));
        player2.addScore(static_cast<int>(p2Score));

        // Store last actions for next round
        p1LastAction = p1Action;
        p2LastAction = p2Action;

        // Print round info
        std::cout << "Round " << round << ": " << "PLAYER: " << player1.getID() << " chose " << (p1Cooperated ? "Cooperate" : "Defect")
            << ", " << "PLAYER: " << player2.getID() << " chose " << (p2Cooperated ? "Cooperate" : "Defect")
            << " | Scores: " << player1.getScore() << " - " << player2.getScore() << "\n";
    }

    printResults();
}

void GameManager::printResults() {
    std::cout << "\nResults:\n";
    std::cout << "PLAYER " << player1.getID() << " (" << player1.getStrategy() << ") - Total Score: " << player1.getScore() << "\n";
    std::cout << "PLAYER " << player2.getID() << " (" << player2.getStrategy() << ") - Total Score: " << player2.getScore() << "\n";
}