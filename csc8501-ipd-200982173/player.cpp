#include "player.hpp"

Player::Player(int identifier, string ipdStrat) {
	playerID = identifier;
	strategy = ipdStrat;
	score = 0;
}

int Player::getID() {
	return playerID;
}

string Player::getStrategy() {
	return strategy;
}

int Player::getScore() {
	return score;
}

void Player::addScore(int newScore) {
	score += newScore;
}