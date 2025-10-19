#pragma once
#include <type_traits>
#include <concepts>

template <typename T>
    requires std::is_arithmetic_v<T>
class Payoff {
public:
    Payoff(T t, T r, T p, T s);

    T calculatePayoff(bool cooperate, bool enemyCooperate) const;

    T getT() const { return tVal; }
    T getR() const { return rVal; }
    T getP() const { return pVal; }
    T getS() const { return sVal; }

    void setT(T newT) { tVal = newT; }
    void setR(T newR) { rVal = newR; }
    void setP(T newP) { pVal = newP; }
    void setS(T newS) { sVal = newS; }

private:
    T tVal; // temptation - player defects while the opponent cooperates (DC)
    T rVal; // reward - both cooperate (CC)
    T pVal; // punishment - both defect (DD)
    T sVal; // sucker - player cooperates while opponent defects (CD)
};

#include "payoff.tpp"
