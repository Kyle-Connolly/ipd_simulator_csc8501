#pragma once

#include "strategy.hpp"

class ALLC : public Strategy {
public:
    Action decideAction() override;
    std::string name() const override;
};