#pragma once

class Payoff {
public:
	Payoff(double t, double r, double p, double s);

    double calculatePayoff(bool cooperated, bool opponentCooperated) const;

    double getT() const;
    double getR() const;
    double getP() const;
    double getS() const;

    void setT(double newT);
    void setR(double newR);
    void setP(double newP);
    void setS(double newS);

private:
    double tVal; // temptation - player defects while the opponent cooperates (DC)
    double rVal; // reward - both cooperate (CC)
    double pVal; // punishment - both defect (DD)
    double sVal; // sucker - player cooperates while opponent defects (CD)
};
