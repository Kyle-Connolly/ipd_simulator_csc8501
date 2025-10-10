#pragma once
#include "strategy.hpp"

class PAVLOV : public Strategy {
public:
    Action decideAction() override;
    std::string name() const override;
};