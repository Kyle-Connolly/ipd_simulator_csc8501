#pragma once

template <typename T>
requires std::is_arithmetic_v<T>
Payoff<T>::Payoff(T t, T r, T p, T s)
    : tVal(t), rVal(r), pVal(p), sVal(s) {}

template <typename T>
requires std::is_arithmetic_v<T>
T Payoff<T>::calculatePayoff(bool cooperate, bool enemyCooperate) const {
    if (cooperate && enemyCooperate)
        return rVal;
    else if (cooperate && !enemyCooperate)
        return sVal;
    else if (!cooperate && enemyCooperate)
        return tVal;
    else
        return pVal;
}