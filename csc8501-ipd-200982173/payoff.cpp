#include "payoff.hpp"

Payoff::Payoff(double t, double r, double p, double s) {
    tVal = t;
	rVal = r;
	pVal = p;
	sVal = s;
}

double Payoff::calculatePayoff(bool cooperate, bool enemyCooperate) {
    if (cooperate && enemyCooperate) {
        return rVal;  // Both cooperate
    }
    else if (cooperate && !enemyCooperate) {
        return sVal;  // Player cooperates, enemy defects
    }
    else if (!cooperate && enemyCooperate) {
        return tVal;  // Player defects, enemy cooperates
    }
    else {
        return pVal;  // Both defect
    }
}

double Payoff::getT() {
    return tVal;
}

double Payoff::getR() {
    return rVal;
}

double Payoff::getP() {
    return pVal;
}

double Payoff::getS() {
    return sVal;
}


void Payoff::setT(double newT) {
    tVal = newT;
}

void Payoff::setR(double newR) {
    rVal = newR;
}

void Payoff::setP(double newP) {
    pVal = newP;
}

void Payoff::setS(double newS) {
    sVal = newS;
}