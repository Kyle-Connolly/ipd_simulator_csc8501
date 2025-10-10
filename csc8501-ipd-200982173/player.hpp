#pragma once
#include <string>
using std::string;

class Player {
public:
	Player(int identifier, string ipdStrat);

	int getID();
	string getStrategy();
	int getScore();

	void addScore(int scoreToAdd);
private:
	int playerID;
	string strategy;
	int score;
};