#include <iostream>
#include "game_manager.hpp"
#include "game_state.hpp"
#include "ctft_strategy.hpp"
#include "prober_strategy.hpp"

GameManager::GameManager(std::unique_ptr<Strategy> s1, std::unique_ptr<Strategy> s2, const Payoff& payoff, double epsilon, std::mt19937& randNumGen, bool noiseOn)
    : player1Strategy(std::move(s1)),
    player2Strategy(std::move(s2)),
    payoffSystem(payoff),
    epsilon(epsilon),
    noiseOn(noiseOn),
    randNumGen(randNumGen)
{}

void GameManager::runGame(int rounds, int repetition, int totalRepeats) {
    Action p1LastAction = Action::Cooperate;
    Action p2LastAction = Action::Cooperate;
    bool p1OpponentDefected = false;
    bool p2OpponentDefected = false;
    
    player1Strategy->resetScore();
    player2Strategy->resetScore();

    std::cout << "----------------------------------";
    std::cout << "\nNext match: " << player1Strategy->name() << " vs " << player2Strategy->name() << "\nRepetition " << repetition << " of " << totalRepeats << "\n\n";

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

        bool p1ActionFlipped = false;
        bool p2ActionFlipped = false;

        if (noiseOn) {
            bool p1IsProber = (dynamic_cast<PROBER*>(player1Strategy.get()) != nullptr);
            auto* p1CtftStrat = dynamic_cast<CTFT*>(player1Strategy.get());
            Action p1OriginalAction = p1Action;
            if (!p1IsProber) {
                bool p1EnableNoise = (distribution(randNumGen) < epsilon);
                // Don't apply noise on the first round
                if (!state1.firstRound && p1EnableNoise) {
                    if (!p1CtftStrat || !p1CtftStrat->isContrite()) {
                        p1Action = (p1Action == Action::Cooperate) ? Action::Defect : Action::Cooperate;
                        p1ActionFlipped = true;
                    }
                }
            }
			// Record intended + actual move for CTFT strategy
            if (p1CtftStrat) {
                p1CtftStrat->setLastMoves(p1OriginalAction, p1Action);
            }

            bool p2IsProber = (dynamic_cast<PROBER*>(player2Strategy.get()) != nullptr);
            auto* p2CtftStrat = dynamic_cast<CTFT*>(player2Strategy.get());
            Action p2OriginalAction = p2Action;
            if (!p2IsProber) {
                bool p2EnableNoise = (distribution(randNumGen) < epsilon);
                if (!state2.firstRound && p2EnableNoise) {
                    if (!p2CtftStrat || !p2CtftStrat->isContrite()) {
                        p2Action = (p2Action == Action::Cooperate) ? Action::Defect : Action::Cooperate;
                        p2ActionFlipped = true;
                    }
                }
            }
            if (p2CtftStrat) {
                p2CtftStrat->setLastMoves(p2OriginalAction, p2Action);
            }
        }
        else {
            // No noise case — still record intended = actual
            if (auto* p1CtftStrat = dynamic_cast<CTFT*>(player1Strategy.get()))
                p1CtftStrat->setLastMoves(p1Action, p1Action);
            if (auto* p2CtftStrat = dynamic_cast<CTFT*>(player2Strategy.get()))
                p2CtftStrat->setLastMoves(p2Action, p2Action);
        }

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

        player1Strategy->addScore(p1Score);
        player2Strategy->addScore(p2Score);

        // Store last actions for next round
        p1LastAction = p1Action;
        p2LastAction = p2Action;

        // Print round info
        std::cout << "Round " << round << ": "
            << player1Strategy->name() << " chose " << (p1Cooperated ? "Cooperate" : "Defect")
            << ", " << player2Strategy->name() << " chose " << (p2Cooperated ? "Cooperate" : "Defect")
            << " | Scores: " << player1Strategy->getScore()
            << " - " << player2Strategy->getScore() << "\n";
        
        // Only report flip if noise caused it
        /*if (p1ActionFlipped) {
            std::cout << player1Strategy->name() << " FLIPPED\n";
        }
        if (p2ActionFlipped) {
            std::cout << player2Strategy->name() << " FLIPPED\n";
        }*/

    }

    printResults();
}

void GameManager::printResults() const {
    std::cout << "\nResults:\n";
    std::cout << player1Strategy->name() << " - Total Score: " << player1Strategy->getScore() << "\n";
    std::cout << player2Strategy->name() << " - Total Score: " << player2Strategy->getScore() << "\n";
}