#pragma once
#include "strategy.hpp"
#include <random>

class RND : public Strategy {
public:
    RND(double probability);
    Action decideAction() override;
    std::string name() const override;
private:
    double p;
    std::mt19937 randNum;
    std::uniform_real_distribution<double> rangeLimit;
};